#include <stdexcept>
#include <cstdlib>
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "leveldb/db.h"

#include <atomic>
#include <cinttypes>
#include <string>

#include "gtest/gtest.h"
#include "db/db_impl.h"
#include "db/filename.h"
#include "db/version_set.h"
#include "db/write_batch_internal.h"
#include "leveldb/cache.h"
#include "leveldb/env.h"
#include "leveldb/filter_policy.h"
#include "leveldb/table.h"
#include "port/port.h"
#include "port/thread_annotations.h"
#include "util/hash.h"
#include "util/logging.h"
#include "util/mutexlock.h"
#include "util/testutil.h"

namespace leveldb {

static std::string RandomString(Random* rnd, int len) {
  std::string r;
  test::RandomString(rnd, len, &r);
  return r;
}

static std::string RandomKey(Random* rnd) {
  int len =
      (rnd->OneIn(3) ? 1  // Short sometimes to encourage collisions
                     : (rnd->OneIn(100) ? rnd->Skewed(10) : rnd->Uniform(10)));
  return test::RandomKey(rnd, len);
}

namespace {
class AtomicCounter {
 public:
  AtomicCounter() : count_(0) {}
  void Increment() { IncrementBy(1); }
  void IncrementBy(int count) LOCKS_EXCLUDED(mu_) {
    MutexLock l(&mu_);
    count_ += count;
  }
  int Read() LOCKS_EXCLUDED(mu_) {
    MutexLock l(&mu_);
    return count_;
  }
  void Reset() LOCKS_EXCLUDED(mu_) {
    MutexLock l(&mu_);
    count_ = 0;
  }

 private:
  port::Mutex mu_;
  int count_ GUARDED_BY(mu_);
};

void DelayMilliseconds(int millis) {
  Env::Default()->SleepForMicroseconds(millis * 1000);
}

bool IsLdbFile(const std::string& f) {
  return strstr(f.c_str(), ".ldb") != nullptr;
}

bool IsLogFile(const std::string& f) {
  return strstr(f.c_str(), ".log") != nullptr;
}

bool IsManifestFile(const std::string& f) {
  return strstr(f.c_str(), "MANIFEST") != nullptr;
}

}  // namespace

// Test Env to override default Env behavior for testing.
class TestEnv : public EnvWrapper {
 public:
  explicit TestEnv(Env* base) : EnvWrapper(base), ignore_dot_files_(false) {}

  void SetIgnoreDotFiles(bool ignored) { ignore_dot_files_ = ignored; }

  Status GetChildren(const std::string& dir,
                     std::vector<std::string>* result) override {
    Status s = target()->GetChildren(dir, result);
    if (!s.ok() || !ignore_dot_files_) {
      return s;
    }

    std::vector<std::string>::iterator it = result->begin();
    while (it != result->end()) {
      if ((*it == ".") || (*it == "..")) {
        it = result->erase(it);
      } else {
        ++it;
      }
    }

    return s;
  }

 private:
  bool ignore_dot_files_;
};

// Special Env used to delay background operations.
class SpecialEnv : public EnvWrapper {
 public:
  // For historical reasons, the std::atomic<> fields below are currently
  // accessed via acquired loads and release stores. We should switch
  // to plain load(), store() calls that provide sequential consistency.

  // sstable/log Sync() calls are blocked while this pointer is non-null.
  std::atomic<bool> delay_data_sync_;

  // sstable/log Sync() calls return an error.
  std::atomic<bool> data_sync_error_;

  // Simulate no-space errors while this pointer is non-null.
  std::atomic<bool> no_space_;

  // Simulate non-writable file system while this pointer is non-null.
  std::atomic<bool> non_writable_;

  // Force sync of manifest files to fail while this pointer is non-null.
  std::atomic<bool> manifest_sync_error_;

  // Force write to manifest files to fail while this pointer is non-null.
  std::atomic<bool> manifest_write_error_;

  // Force log file close to fail while this bool is true.
  std::atomic<bool> log_file_close_;

  bool count_random_reads_;
  AtomicCounter random_read_counter_;

  explicit SpecialEnv(Env* base)
      : EnvWrapper(base),
        delay_data_sync_(false),
        data_sync_error_(false),
        no_space_(false),
        non_writable_(false),
        manifest_sync_error_(false),
        manifest_write_error_(false),
        log_file_close_(false),
        count_random_reads_(false) {}

  Status NewWritableFile(const std::string& f, WritableFile** r) {
    class DataFile : public WritableFile {
     private:
      SpecialEnv* const env_;
      WritableFile* const base_;
      const std::string fname_;

     public:
      DataFile(SpecialEnv* env, WritableFile* base, const std::string& fname)
          : env_(env), base_(base), fname_(fname) {}

      ~DataFile() { delete base_; }
      Status Append(const Slice& data) {
        if (env_->no_space_.load(std::memory_order_acquire)) {
          // Drop writes on the floor
          return Status::OK();
        } else {
          return base_->Append(data);
        }
      }
      Status Close() {
        Status s = base_->Close();
        if (s.ok() && IsLogFile(fname_) &&
            env_->log_file_close_.load(std::memory_order_acquire)) {
          s = Status::IOError("simulated log file Close error");
        }
        return s;
      }
      Status Flush() { return base_->Flush(); }
      Status Sync() {
        if (env_->data_sync_error_.load(std::memory_order_acquire)) {
          return Status::IOError("simulated data sync error");
        }
        while (env_->delay_data_sync_.load(std::memory_order_acquire)) {
          DelayMilliseconds(100);
        }
        return base_->Sync();
      }
    };
    class ManifestFile : public WritableFile {
     private:
      SpecialEnv* env_;
      WritableFile* base_;

     public:
      ManifestFile(SpecialEnv* env, WritableFile* b) : env_(env), base_(b) {}
      ~ManifestFile() { delete base_; }
      Status Append(const Slice& data) {
        if (env_->manifest_write_error_.load(std::memory_order_acquire)) {
          return Status::IOError("simulated writer error");
        } else {
          return base_->Append(data);
        }
      }
      Status Close() { return base_->Close(); }
      Status Flush() { return base_->Flush(); }
      Status Sync() {
        if (env_->manifest_sync_error_.load(std::memory_order_acquire)) {
          return Status::IOError("simulated sync error");
        } else {
          return base_->Sync();
        }
      }
    };

    if (non_writable_.load(std::memory_order_acquire)) {
      return Status::IOError("simulated write error");
    }

    Status s = target()->NewWritableFile(f, r);
    if (s.ok()) {
      if (IsLdbFile(f) || IsLogFile(f)) {
        *r = new DataFile(this, *r, f);
      } else if (IsManifestFile(f)) {
        *r = new ManifestFile(this, *r);
      }
    }
    return s;
  }

  Status NewRandomAccessFile(const std::string& f, RandomAccessFile** r) {
    class CountingFile : public RandomAccessFile {
     private:
      RandomAccessFile* target_;
      AtomicCounter* counter_;

     public:
      CountingFile(RandomAccessFile* target, AtomicCounter* counter)
          : target_(target), counter_(counter) {}
      ~CountingFile() override { delete target_; }
      Status Read(uint64_t offset, size_t n, Slice* result,
                  char* scratch) const override {
        counter_->Increment();
        return target_->Read(offset, n, result, scratch);
      }
    };

    Status s = target()->NewRandomAccessFile(f, r);
    if (s.ok() && count_random_reads_) {
      *r = new CountingFile(*r, &random_read_counter_);
    }
    return s;
  }
};

class DBTest : public testing::Test {
 public:
  std::string dbname_;
  SpecialEnv* env_;
  DB* db_;

  Options last_options_;

  DBTest() : env_(new SpecialEnv(Env::Default())), option_config_(kDefault) {
    filter_policy_ = NewBloomFilterPolicy(10);
    dbname_ = testing::TempDir() + "db_test";
    DestroyDB(dbname_, Options());
    db_ = nullptr;
    Reopen();
  }

  ~DBTest() {
    delete db_;
    DestroyDB(dbname_, Options());
    delete env_;
    delete filter_policy_;
  }

  // Switch to a fresh database with the next option configuration to
  // test.  Return false if there are no more configurations to test.
  bool ChangeOptions() {
    option_config_++;
    if (option_config_ >= kEnd) {
      return false;
    } else {
      DestroyAndReopen();
      return true;
    }
  }

  // Return the current option configuration.
  Options CurrentOptions() {
    Options options;
    options.reuse_logs = false;
    switch (option_config_) {
      case kReuse:
        options.reuse_logs = true;
        break;
      case kFilter:
        options.filter_policy = filter_policy_;
        break;
      case kUncompressed:
        options.compression = kNoCompression;
        break;
      default:
        break;
    }
    return options;
  }

  DBImpl* dbfull() { return reinterpret_cast<DBImpl*>(db_); }

  void Reopen(Options* options = nullptr) {
    ASSERT_LEVELDB_OK(TryReopen(options));
  }

  void Close() {
    delete db_;
    db_ = nullptr;
  }

  void DestroyAndReopen(Options* options = nullptr) {
    delete db_;
    db_ = nullptr;
    DestroyDB(dbname_, Options());
    ASSERT_LEVELDB_OK(TryReopen(options));
  }

  Status TryReopen(Options* options) {
    delete db_;
    db_ = nullptr;
    Options opts;
    if (options != nullptr) {
      opts = *options;
    } else {
      opts = CurrentOptions();
      opts.create_if_missing = true;
    }
    last_options_ = opts;

    return DB::Open(opts, dbname_, &db_);
  }

  Status Put(const std::string& k, const std::string& v) {
    return db_->Put(WriteOptions(), k, v);
  }

  Status Delete(const std::string& k) { return db_->Delete(WriteOptions(), k); }

  std::string Get(const std::string& k, const Snapshot* snapshot = nullptr) {
    ReadOptions options;
    options.snapshot = snapshot;
    std::string result;
    Status s = db_->Get(options, k, &result);
    if (s.IsNotFound()) {
      result = "NOT_FOUND";
    } else if (!s.ok()) {
      result = s.ToString();
    }
    return result;
  }

  // Return a string that contains all key,value pairs in order,
  // formatted like "(k1->v1)(k2->v2)".
  std::string Contents() {
    std::vector<std::string> forward;
    std::string result;
    Iterator* iter = db_->NewIterator(ReadOptions());
    for (iter->SeekToFirst(); iter->Valid(); iter->Next()) {
      std::string s = IterStatus(iter);
      result.push_back('(');
      result.append(s);
      result.push_back(')');
      forward.push_back(s);
    }

    // Check reverse iteration results are the reverse of forward results
    size_t matched = 0;
    for (iter->SeekToLast(); iter->Valid(); iter->Prev()) {
      EXPECT_LT(matched, forward.size());
      EXPECT_EQ(IterStatus(iter), forward[forward.size() - matched - 1]);
      matched++;
    }
    EXPECT_EQ(matched, forward.size());

    delete iter;
    return result;
  }

  std::string AllEntriesFor(const Slice& user_key) {
    Iterator* iter = dbfull()->TEST_NewInternalIterator();
    InternalKey target(user_key, kMaxSequenceNumber, kTypeValue);
    iter->Seek(target.Encode());
    std::string result;
    if (!iter->status().ok()) {
      result = iter->status().ToString();
    } else {
      result = "[ ";
      bool first = true;
      while (iter->Valid()) {
        ParsedInternalKey ikey;
        if (!ParseInternalKey(iter->key(), &ikey)) {
          result += "CORRUPTED";
        } else {
          if (last_options_.comparator->Compare(ikey.user_key, user_key) != 0) {
            break;
          }
          if (!first) {
            result += ", ";
          }
          first = false;
          switch (ikey.type) {
            case kTypeValue:
              result += iter->value().ToString();
              break;
            case kTypeDeletion:
              result += "DEL";
              break;
          }
        }
        iter->Next();
      }
      if (!first) {
        result += " ";
      }
      result += "]";
    }
    delete iter;
    return result;
  }

  int NumTableFilesAtLevel(int level) {
    std::string property;
    EXPECT_TRUE(db_->GetProperty(
        "leveldb.num-files-at-level" + NumberToString(level), &property));
    return std::stoi(property);
  }

  int TotalTableFiles() {
    int result = 0;
    for (int level = 0; level < config::kNumLevels; level++) {
      result += NumTableFilesAtLevel(level);
    }
    return result;
  }

  // Return spread of files per level
  std::string FilesPerLevel() {
    std::string result;
    int last_non_zero_offset = 0;
    for (int level = 0; level < config::kNumLevels; level++) {
      int f = NumTableFilesAtLevel(level);
      char buf[100];
      std::snprintf(buf, sizeof(buf), "%s%d", (level ? "," : ""), f);
      result += buf;
      if (f > 0) {
        last_non_zero_offset = result.size();
      }
    }
    result.resize(last_non_zero_offset);
    return result;
  }

  int CountFiles() {
    std::vector<std::string> files;
    env_->GetChildren(dbname_, &files);
    return static_cast<int>(files.size());
  }

  uint64_t Size(const Slice& start, const Slice& limit) {
    Range r(start, limit);
    uint64_t size;
    db_->GetApproximateSizes(&r, 1, &size);
    return size;
  }

  void Compact(const Slice& start, const Slice& limit) {
    db_->CompactRange(&start, &limit);
  }

  // Do n memtable compactions, each of which produces an sstable
  // covering the range [small_key,large_key].
  void MakeTables(int n, const std::string& small_key,
                  const std::string& large_key) {
    for (int i = 0; i < n; i++) {
      Put(small_key, "begin");
      Put(large_key, "end");
      dbfull()->TEST_CompactMemTable();
    }
  }

  // Prevent pushing of new sstables into deeper levels by adding
  // tables that cover a specified range to all levels.
  void FillLevels(const std::string& smallest, const std::string& largest) {
    MakeTables(config::kNumLevels, smallest, largest);
  }

  void DumpFileCounts(const char* label) {
    std::fprintf(stderr, "---\n%s:\n", label);
    std::fprintf(
        stderr, "maxoverlap: %lld\n",
        static_cast<long long>(dbfull()->TEST_MaxNextLevelOverlappingBytes()));
    for (int level = 0; level < config::kNumLevels; level++) {
      int num = NumTableFilesAtLevel(level);
      if (num > 0) {
        std::fprintf(stderr, "  level %3d : %d files\n", level, num);
      }
    }
  }

  std::string DumpSSTableList() {
    std::string property;
    db_->GetProperty("leveldb.sstables", &property);
    return property;
  }

  std::string IterStatus(Iterator* iter) {
    std::string result;
    if (iter->Valid()) {
      result = iter->key().ToString() + "->" + iter->value().ToString();
    } else {
      result = "(invalid)";
    }
    return result;
  }

  bool DeleteAnSSTFile() {
    std::vector<std::string> filenames;
    EXPECT_LEVELDB_OK(env_->GetChildren(dbname_, &filenames));
    uint64_t number;
    FileType type;
    for (size_t i = 0; i < filenames.size(); i++) {
      if (ParseFileName(filenames[i], &number, &type) && type == kTableFile) {
        EXPECT_LEVELDB_OK(env_->RemoveFile(TableFileName(dbname_, number)));
        return true;
      }
    }
    return false;
  }

  // Returns number of files renamed.
  int RenameLDBToSST() {
    std::vector<std::string> filenames;
    EXPECT_LEVELDB_OK(env_->GetChildren(dbname_, &filenames));
    uint64_t number;
    FileType type;
    int files_renamed = 0;
    for (size_t i = 0; i < filenames.size(); i++) {
      if (ParseFileName(filenames[i], &number, &type) && type == kTableFile) {
        const std::string from = TableFileName(dbname_, number);
        const std::string to = SSTTableFileName(dbname_, number);
        EXPECT_LEVELDB_OK(env_->RenameFile(from, to));
        files_renamed++;
      }
    }
    return files_renamed;
  }

 private:
  // Sequence of option configurations to try
  enum OptionConfig { kDefault, kReuse, kFilter, kUncompressed, kEnd };

  const FilterPolicy* filter_policy_;
  int option_config_;
};

TEST_F(DBTest, Empty) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, EmptyKey) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, EmptyValue) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, ReadWrite) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, PutDeleteGet) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, GetFromImmutableLayer) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, GetFromVersions) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, GetMemUsage) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, GetSnapshot) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, GetIdenticalSnapshots) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, IterateOverEmptySnapshot) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, GetLevel0Ordering) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, GetOrderedByLevels) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, GetPicksCorrectFile) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, GetEncountersEmptyLevel) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, IterEmpty) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, IterSingle) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, IterMulti) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, IterSmallAndLargeMix) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, IterMultiWithDelete) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, IterMultiWithDeleteAndCompaction) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, Recover) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, RecoveryWithEmptyLog) {
    __builtin_trap() /* STUB: not implemented */;
}

// Check that writes done during a memtable compaction are recovered
// if the database is shutdown during the memtable compaction.
TEST_F(DBTest, RecoverDuringMemtableCompaction) {
    __builtin_trap() /* STUB: not implemented */;
}

static std::string Key(int i) {
  char buf[100];
  std::snprintf(buf, sizeof(buf), "key%06d", i);
  return std::string(buf);
}

TEST_F(DBTest, MinorCompactionsHappen) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, RecoverWithLargeLog) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, CompactionsGenerateMultipleFiles) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, RepeatedWritesToSameKey) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, SparseMerge) {
    __builtin_trap() /* STUB: not implemented */;
}

static bool Between(uint64_t val, uint64_t low, uint64_t high) {
  bool result = (val >= low) && (val <= high);
  if (!result) {
    std::fprintf(stderr, "Value %llu is not in range [%llu, %llu]\n",
                 (unsigned long long)(val), (unsigned long long)(low),
                 (unsigned long long)(high));
  }
  return result;
}

TEST_F(DBTest, ApproximateSizes) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, ApproximateSizes_MixOfSmallAndLarge) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, IteratorPinsRef) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, Snapshot) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, HiddenValuesAreRemoved) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, DeletionMarkers1) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, DeletionMarkers2) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, OverlapInLevel0) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, L0_CompactionBug_Issue44_a) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, L0_CompactionBug_Issue44_b) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, Fflush_Issue474) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, ComparatorCheck) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, CustomComparator) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, ManualCompaction) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, DBOpen_Options) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, DestroyEmptyDir) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, DestroyOpenDB) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, Locking) {
    __builtin_trap() /* STUB: not implemented */;
}

// Check that number of files does not grow when we are out of space
TEST_F(DBTest, NoSpace) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, NonWritableFileSystem) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, WriteSyncError) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, ManifestWriteError) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, MissingSSTFile) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, StillReadSST) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, FilesDeletedAfterCompaction) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, BloomFilter) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(DBTest, LogCloseError) {
    __builtin_trap() /* STUB: not implemented */;
}

// Multi-threaded test:
namespace {

static const int kNumThreads = 4;
static const int kTestSeconds = 10;
static const int kNumKeys = 1000;

struct MTState {
  DBTest* test;
  std::atomic<bool> stop;
  std::atomic<int> counter[kNumThreads];
  std::atomic<bool> thread_done[kNumThreads];
};

struct MTThread {
  MTState* state;
  int id;
};

static void MTThreadBody(void* arg) {
  MTThread* t = reinterpret_cast<MTThread*>(arg);
  int id = t->id;
  DB* db = t->state->test->db_;
  int counter = 0;
  std::fprintf(stderr, "... starting thread %d\n", id);
  Random rnd(1000 + id);
  std::string value;
  char valbuf[1500];
  while (!t->state->stop.load(std::memory_order_acquire)) {
    t->state->counter[id].store(counter, std::memory_order_release);

    int key = rnd.Uniform(kNumKeys);
    char keybuf[20];
    std::snprintf(keybuf, sizeof(keybuf), "%016d", key);

    if (rnd.OneIn(2)) {
      // Write values of the form <key, my id, counter>.
      // We add some padding for force compactions.
      std::snprintf(valbuf, sizeof(valbuf), "%d.%d.%-1000d", key, id,
                    static_cast<int>(counter));
      ASSERT_LEVELDB_OK(db->Put(WriteOptions(), Slice(keybuf), Slice(valbuf)));
    } else {
      // Read a value and verify that it matches the pattern written above.
      Status s = db->Get(ReadOptions(), Slice(keybuf), &value);
      if (s.IsNotFound()) {
        // Key has not yet been written
      } else {
        // Check that the writer thread counter is >= the counter in the value
        ASSERT_LEVELDB_OK(s);
        int k, w, c;
        ASSERT_EQ(3, sscanf(value.c_str(), "%d.%d.%d", &k, &w, &c)) << value;
        ASSERT_EQ(k, key);
        ASSERT_GE(w, 0);
        ASSERT_LT(w, kNumThreads);
        ASSERT_LE(c, t->state->counter[w].load(std::memory_order_acquire));
      }
    }
    counter++;
  }
  t->state->thread_done[id].store(true, std::memory_order_release);
  std::fprintf(stderr, "... stopping thread %d after %d ops\n", id, counter);
}

}  // namespace

TEST_F(DBTest, MultiThreaded) {
    __builtin_trap() /* STUB: not implemented */;
}

namespace {
typedef std::map<std::string, std::string> KVMap;
}

class ModelDB : public DB {
 public:
  class ModelSnapshot : public Snapshot {
   public:
    KVMap map_;
  };

  explicit ModelDB(const Options& options) : options_(options) {}
  ~ModelDB() override = default;
  Status Put(const WriteOptions& o, const Slice& k, const Slice& v) override {
    return DB::Put(o, k, v);
  }
  Status Delete(const WriteOptions& o, const Slice& key) override {
    return DB::Delete(o, key);
  }
  Status Get(const ReadOptions& options, const Slice& key,
             std::string* value) override {
    assert(false);  // Not implemented
    return Status::NotFound(key);
  }
  Iterator* NewIterator(const ReadOptions& options) override {
    if (options.snapshot == nullptr) {
      KVMap* saved = new KVMap;
      *saved = map_;
      return new ModelIter(saved, true);
    } else {
      const KVMap* snapshot_state =
          &(reinterpret_cast<const ModelSnapshot*>(options.snapshot)->map_);
      return new ModelIter(snapshot_state, false);
    }
  }
  const Snapshot* GetSnapshot() override {
    ModelSnapshot* snapshot = new ModelSnapshot;
    snapshot->map_ = map_;
    return snapshot;
  }

  void ReleaseSnapshot(const Snapshot* snapshot) override {
    delete reinterpret_cast<const ModelSnapshot*>(snapshot);
  }
  Status Write(const WriteOptions& options, WriteBatch* batch) override {
    class Handler : public WriteBatch::Handler {
     public:
      KVMap* map_;
      void Put(const Slice& key, const Slice& value) override {
        (*map_)[key.ToString()] = value.ToString();
      }
      void Delete(const Slice& key) override { map_->erase(key.ToString()); }
    };
    Handler handler;
    handler.map_ = &map_;
    return batch->Iterate(&handler);
  }

  bool GetProperty(const Slice& property, std::string* value) override {
    return false;
  }
  void GetApproximateSizes(const Range* r, int n, uint64_t* sizes) override {
    for (int i = 0; i < n; i++) {
      sizes[i] = 0;
    }
  }
  void CompactRange(const Slice* start, const Slice* end) override {}

 private:
  class ModelIter : public Iterator {
   public:
    ModelIter(const KVMap* map, bool owned)
        : map_(map), owned_(owned), iter_(map_->end()) {}
    ~ModelIter() override {
      if (owned_) delete map_;
    }
    bool Valid() const override { return iter_ != map_->end(); }
    void SeekToFirst() override { iter_ = map_->begin(); }
    void SeekToLast() override {
      if (map_->empty()) {
        iter_ = map_->end();
      } else {
        iter_ = map_->find(map_->rbegin()->first);
      }
    }
    void Seek(const Slice& k) override {
      iter_ = map_->lower_bound(k.ToString());
    }
    void Next() override { ++iter_; }
    void Prev() override { --iter_; }
    Slice key() const override { return iter_->first; }
    Slice value() const override { return iter_->second; }
    Status status() const override { return Status::OK(); }

   private:
    const KVMap* const map_;
    const bool owned_;  // Do we own map_
    KVMap::const_iterator iter_;
  };
  const Options options_;
  KVMap map_;
};

static bool CompareIterators(int step, DB* model, DB* db,
                             const Snapshot* model_snap,
                             const Snapshot* db_snap) {
  ReadOptions options;
  options.snapshot = model_snap;
  Iterator* miter = model->NewIterator(options);
  options.snapshot = db_snap;
  Iterator* dbiter = db->NewIterator(options);
  bool ok = true;
  int count = 0;
  std::vector<std::string> seek_keys;
  // Compare equality of all elements using Next(). Save some of the keys for
  // comparing Seek equality.
  for (miter->SeekToFirst(), dbiter->SeekToFirst();
       ok && miter->Valid() && dbiter->Valid(); miter->Next(), dbiter->Next()) {
    count++;
    if (miter->key().compare(dbiter->key()) != 0) {
      std::fprintf(stderr, "step %d: Key mismatch: '%s' vs. '%s'\n", step,
                   EscapeString(miter->key()).c_str(),
                   EscapeString(dbiter->key()).c_str());
      ok = false;
      break;
    }

    if (miter->value().compare(dbiter->value()) != 0) {
      std::fprintf(stderr,
                   "step %d: Value mismatch for key '%s': '%s' vs. '%s'\n",
                   step, EscapeString(miter->key()).c_str(),
                   EscapeString(miter->value()).c_str(),
                   EscapeString(miter->value()).c_str());
      ok = false;
      break;
    }

    if (count % 10 == 0) {
      seek_keys.push_back(miter->key().ToString());
    }
  }

  if (ok) {
    if (miter->Valid() != dbiter->Valid()) {
      std::fprintf(stderr, "step %d: Mismatch at end of iterators: %d vs. %d\n",
                   step, miter->Valid(), dbiter->Valid());
      ok = false;
    }
  }

  if (ok) {
    // Validate iterator equality when performing seeks.
    for (auto kiter = seek_keys.begin(); ok && kiter != seek_keys.end();
         ++kiter) {
      miter->Seek(*kiter);
      dbiter->Seek(*kiter);
      if (!miter->Valid() || !dbiter->Valid()) {
        std::fprintf(stderr, "step %d: Seek iterators invalid: %d vs. %d\n",
                     step, miter->Valid(), dbiter->Valid());
        ok = false;
      }
      if (miter->key().compare(dbiter->key()) != 0) {
        std::fprintf(stderr, "step %d: Seek key mismatch: '%s' vs. '%s'\n",
                     step, EscapeString(miter->key()).c_str(),
                     EscapeString(dbiter->key()).c_str());
        ok = false;
        break;
      }

      if (miter->value().compare(dbiter->value()) != 0) {
        std::fprintf(
            stderr,
            "step %d: Seek value mismatch for key '%s': '%s' vs. '%s'\n", step,
            EscapeString(miter->key()).c_str(),
            EscapeString(miter->value()).c_str(),
            EscapeString(miter->value()).c_str());
        ok = false;
        break;
      }
    }
  }

  std::fprintf(stderr, "%d entries compared: ok=%d\n", count, ok);
  delete miter;
  delete dbiter;
  return ok;
}

TEST_F(DBTest, Randomized) {
    __builtin_trap() /* STUB: not implemented */;
}

}  // namespace leveldb
