#include <stdexcept>
#include <cstdlib>
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include <sys/types.h>

#include "gtest/gtest.h"
#include "db/db_impl.h"
#include "db/filename.h"
#include "db/log_format.h"
#include "db/version_set.h"
#include "leveldb/cache.h"
#include "leveldb/db.h"
#include "leveldb/table.h"
#include "leveldb/write_batch.h"
#include "util/logging.h"
#include "util/testutil.h"

namespace leveldb {

static const int kValueSize = 1000;

class CorruptionTest : public testing::Test {
 public:
  CorruptionTest()
      : db_(nullptr),
        dbname_("/memenv/corruption_test"),
        tiny_cache_(NewLRUCache(100)) {
    options_.env = &env_;
    options_.block_cache = tiny_cache_;
    DestroyDB(dbname_, options_);

    options_.create_if_missing = true;
    Reopen();
    options_.create_if_missing = false;
  }

  ~CorruptionTest() {
    delete db_;
    delete tiny_cache_;
  }

  Status TryReopen() {
    delete db_;
    db_ = nullptr;
    return DB::Open(options_, dbname_, &db_);
  }

  void Reopen() { ASSERT_LEVELDB_OK(TryReopen()); }

  void RepairDB() {
    delete db_;
    db_ = nullptr;
    ASSERT_LEVELDB_OK(::leveldb::RepairDB(dbname_, options_));
  }

  void Build(int n) {
    std::string key_space, value_space;
    WriteBatch batch;
    for (int i = 0; i < n; i++) {
      // if ((i % 100) == 0) std::fprintf(stderr, "@ %d of %d\n", i, n);
      Slice key = Key(i, &key_space);
      batch.Clear();
      batch.Put(key, Value(i, &value_space));
      WriteOptions options;
      // Corrupt() doesn't work without this sync on windows; stat reports 0 for
      // the file size.
      if (i == n - 1) {
        options.sync = true;
      }
      ASSERT_LEVELDB_OK(db_->Write(options, &batch));
    }
  }

  void Check(int min_expected, int max_expected) {
    int next_expected = 0;
    int missed = 0;
    int bad_keys = 0;
    int bad_values = 0;
    int correct = 0;
    std::string value_space;
    Iterator* iter = db_->NewIterator(ReadOptions());
    for (iter->SeekToFirst(); iter->Valid(); iter->Next()) {
      uint64_t key;
      Slice in(iter->key());
      if (in == "" || in == "~") {
        // Ignore boundary keys.
        continue;
      }
      if (!ConsumeDecimalNumber(&in, &key) || !in.empty() ||
          key < next_expected) {
        bad_keys++;
        continue;
      }
      missed += (key - next_expected);
      next_expected = key + 1;
      if (iter->value() != Value(key, &value_space)) {
        bad_values++;
      } else {
        correct++;
      }
    }
    delete iter;

    std::fprintf(
        stderr,
        "expected=%d..%d; got=%d; bad_keys=%d; bad_values=%d; missed=%d\n",
        min_expected, max_expected, correct, bad_keys, bad_values, missed);
    ASSERT_LE(min_expected, correct);
    ASSERT_GE(max_expected, correct);
  }

  void Corrupt(FileType filetype, int offset, int bytes_to_corrupt) {
    // Pick file to corrupt
    std::vector<std::string> filenames;
    ASSERT_LEVELDB_OK(env_.target()->GetChildren(dbname_, &filenames));
    uint64_t number;
    FileType type;
    std::string fname;
    int picked_number = -1;
    for (size_t i = 0; i < filenames.size(); i++) {
      if (ParseFileName(filenames[i], &number, &type) && type == filetype &&
          int(number) > picked_number) {  // Pick latest file
        fname = dbname_ + "/" + filenames[i];
        picked_number = number;
      }
    }
    ASSERT_TRUE(!fname.empty()) << filetype;

    uint64_t file_size;
    ASSERT_LEVELDB_OK(env_.target()->GetFileSize(fname, &file_size));

    if (offset < 0) {
      // Relative to end of file; make it absolute
      if (-offset > file_size) {
        offset = 0;
      } else {
        offset = file_size + offset;
      }
    }
    if (offset > file_size) {
      offset = file_size;
    }
    if (offset + bytes_to_corrupt > file_size) {
      bytes_to_corrupt = file_size - offset;
    }

    // Do it
    std::string contents;
    Status s = ReadFileToString(env_.target(), fname, &contents);
    ASSERT_TRUE(s.ok()) << s.ToString();
    for (int i = 0; i < bytes_to_corrupt; i++) {
      contents[i + offset] ^= 0x80;
    }
    s = WriteStringToFile(env_.target(), contents, fname);
    ASSERT_TRUE(s.ok()) << s.ToString();
  }

  int Property(const std::string& name) {
    std::string property;
    int result;
    if (db_->GetProperty(name, &property) &&
        sscanf(property.c_str(), "%d", &result) == 1) {
      return result;
    } else {
      return -1;
    }
  }

  // Return the ith key
  Slice Key(int i, std::string* storage) {
    char buf[100];
    std::snprintf(buf, sizeof(buf), "%016d", i);
    storage->assign(buf, strlen(buf));
    return Slice(*storage);
  }

  // Return the value to associate with the specified key
  Slice Value(int k, std::string* storage) {
    Random r(k);
    return test::RandomString(&r, kValueSize, storage);
  }

  test::ErrorEnv env_;
  Options options_;
  DB* db_;

 private:
  std::string dbname_;
  Cache* tiny_cache_;
};

TEST_F(CorruptionTest, Recovery) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(CorruptionTest, RecoverWriteError) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(CorruptionTest, NewFileErrorDuringWrite) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(CorruptionTest, TableFile) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(CorruptionTest, TableFileRepair) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(CorruptionTest, TableFileIndexData) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(CorruptionTest, MissingDescriptor) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(CorruptionTest, SequenceNumberRecovery) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(CorruptionTest, CorruptedDescriptor) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(CorruptionTest, CompactionInputError) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(CorruptionTest, CompactionInputErrorParanoid) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(CorruptionTest, UnrelatedKeys) {
    __builtin_trap() /* STUB: not implemented */;
}

}  // namespace leveldb
