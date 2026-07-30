#include <stdexcept>
#include <cstdlib>
// Copyright (c) 2014 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "gtest/gtest.h"
#include "db/db_impl.h"
#include "db/filename.h"
#include "db/version_set.h"
#include "db/write_batch_internal.h"
#include "leveldb/db.h"
#include "leveldb/env.h"
#include "leveldb/write_batch.h"
#include "util/logging.h"
#include "util/testutil.h"

namespace leveldb {

class RecoveryTest : public testing::Test {
 public:
  RecoveryTest() : env_(Env::Default()), db_(nullptr) {
    dbname_ = testing::TempDir() + "recovery_test";
    DestroyDB(dbname_, Options());
    Open();
  }

  ~RecoveryTest() {
    Close();
    DestroyDB(dbname_, Options());
  }

  DBImpl* dbfull() const { return reinterpret_cast<DBImpl*>(db_); }
  Env* env() const { return env_; }

  bool CanAppend() {
    WritableFile* tmp;
    Status s = env_->NewAppendableFile(CurrentFileName(dbname_), &tmp);
    delete tmp;
    if (s.IsNotSupportedError()) {
      return false;
    } else {
      return true;
    }
  }

  void Close() {
    delete db_;
    db_ = nullptr;
  }

  Status OpenWithStatus(Options* options = nullptr) {
    Close();
    Options opts;
    if (options != nullptr) {
      opts = *options;
    } else {
      opts.reuse_logs = true;  // TODO(sanjay): test both ways
      opts.create_if_missing = true;
    }
    if (opts.env == nullptr) {
      opts.env = env_;
    }
    return DB::Open(opts, dbname_, &db_);
  }

  void Open(Options* options = nullptr) {
    ASSERT_LEVELDB_OK(OpenWithStatus(options));
    ASSERT_EQ(1, NumLogs());
  }

  Status Put(const std::string& k, const std::string& v) {
    return db_->Put(WriteOptions(), k, v);
  }

  std::string Get(const std::string& k, const Snapshot* snapshot = nullptr) {
    std::string result;
    Status s = db_->Get(ReadOptions(), k, &result);
    if (s.IsNotFound()) {
      result = "NOT_FOUND";
    } else if (!s.ok()) {
      result = s.ToString();
    }
    return result;
  }

  std::string ManifestFileName() {
    std::string current;
    EXPECT_LEVELDB_OK(
        ReadFileToString(env_, CurrentFileName(dbname_), &current));
    size_t len = current.size();
    if (len > 0 && current[len - 1] == '\n') {
      current.resize(len - 1);
    }
    return dbname_ + "/" + current;
  }

  std::string LogName(uint64_t number) { return LogFileName(dbname_, number); }

  size_t RemoveLogFiles() {
    // Linux allows unlinking open files, but Windows does not.
    // Closing the db allows for file deletion.
    Close();
    std::vector<uint64_t> logs = GetFiles(kLogFile);
    for (size_t i = 0; i < logs.size(); i++) {
      EXPECT_LEVELDB_OK(env_->RemoveFile(LogName(logs[i]))) << LogName(logs[i]);
    }
    return logs.size();
  }

  void RemoveManifestFile() {
    ASSERT_LEVELDB_OK(env_->RemoveFile(ManifestFileName()));
  }

  uint64_t FirstLogFile() { return GetFiles(kLogFile)[0]; }

  std::vector<uint64_t> GetFiles(FileType t) {
    std::vector<std::string> filenames;
    EXPECT_LEVELDB_OK(env_->GetChildren(dbname_, &filenames));
    std::vector<uint64_t> result;
    for (size_t i = 0; i < filenames.size(); i++) {
      uint64_t number;
      FileType type;
      if (ParseFileName(filenames[i], &number, &type) && type == t) {
        result.push_back(number);
      }
    }
    return result;
  }

  int NumLogs() { return GetFiles(kLogFile).size(); }

  int NumTables() { return GetFiles(kTableFile).size(); }

  uint64_t FileSize(const std::string& fname) {
    uint64_t result;
    EXPECT_LEVELDB_OK(env_->GetFileSize(fname, &result)) << fname;
    return result;
  }

  void CompactMemTable() { dbfull()->TEST_CompactMemTable(); }

  // Directly construct a log file that sets key to val.
  void MakeLogFile(uint64_t lognum, SequenceNumber seq, Slice key, Slice val) {
    std::string fname = LogFileName(dbname_, lognum);
    WritableFile* file;
    ASSERT_LEVELDB_OK(env_->NewWritableFile(fname, &file));
    log::Writer writer(file);
    WriteBatch batch;
    batch.Put(key, val);
    WriteBatchInternal::SetSequence(&batch, seq);
    ASSERT_LEVELDB_OK(writer.AddRecord(WriteBatchInternal::Contents(&batch)));
    ASSERT_LEVELDB_OK(file->Flush());
    delete file;
  }

 private:
  std::string dbname_;
  Env* env_;
  DB* db_;
};

TEST_F(RecoveryTest, ManifestReused) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(RecoveryTest, LargeManifestCompacted) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(RecoveryTest, NoLogFiles) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(RecoveryTest, LogFileReuse) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(RecoveryTest, MultipleMemTables) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(RecoveryTest, MultipleLogFiles) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(RecoveryTest, ManifestMissing) {
    __builtin_trap() /* STUB: not implemented */;
}

}  // namespace leveldb
