// Copyright 2014 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

// This test uses a custom Env to keep track of the state of a filesystem as of
// the last "sync". It then checks for data loss errors by purposely dropping
// file data (or entire files) not protected by a "sync".

#include <map>
#include <set>

#include "gtest/gtest.h"
#include "db/db_impl.h"
#include "db/filename.h"
#include "db/log_format.h"
#include "db/version_set.h"
#include "leveldb/cache.h"
#include "leveldb/db.h"
#include "leveldb/env.h"
#include "leveldb/table.h"
#include "leveldb/write_batch.h"
#include "port/port.h"
#include "port/thread_annotations.h"
#include "util/logging.h"
#include "util/mutexlock.h"
#include "util/testutil.h"

namespace leveldb {

static const int kValueSize = 1000;
static const int kMaxNumValues = 2000;
static const size_t kNumIterations = 3;

class FaultInjectionTestEnv;

namespace {

// Assume a filename, and not a directory name like "/foo/bar/"
static std::string GetDirName(const std::string& filename) { __builtin_trap() /* STUB: not implemented */; }

Status SyncDir(const std::string& dir) { __builtin_trap() /* STUB: not implemented */; }

// A basic file truncation function suitable for this test.
Status Truncate(const std::string& filename, uint64_t length) { __builtin_trap() /* STUB: not implemented */; }

struct FileState {
  std::string filename_;
  int64_t pos_;
  int64_t pos_at_last_sync_;
  int64_t pos_at_last_flush_;

  FileState(const std::string& filename)
      : filename_(filename),
        pos_(-1),
        pos_at_last_sync_(-1),
        pos_at_last_flush_(-1) { __builtin_trap() /* STUB: not implemented */; }

  FileState() : pos_(-1), pos_at_last_sync_(-1), pos_at_last_flush_(-1) { __builtin_trap() /* STUB: not implemented */; }

  bool IsFullySynced() const { __builtin_trap() /* STUB: not implemented */; }

  Status DropUnsyncedData() const;
};

}  // anonymous namespace

// A wrapper around WritableFile which informs another Env whenever this file
// is written to or sync'ed.
class TestWritableFile : public WritableFile {
 public:
  TestWritableFile(const FileState& state, WritableFile* f,
                   FaultInjectionTestEnv* env);
  ~TestWritableFile() override;
  Status Append(const Slice& data) override;
  Status Close() override;
  Status Flush() override;
  Status Sync() override;

 private:
  FileState state_;
  WritableFile* target_;
  bool writable_file_opened_;
  FaultInjectionTestEnv* env_;

  Status SyncParent();
};

class FaultInjectionTestEnv : public EnvWrapper {
 public:
  FaultInjectionTestEnv()
      : EnvWrapper(Env::Default()), filesystem_active_(true) { __builtin_trap() /* STUB: not implemented */; }
  ~FaultInjectionTestEnv() override = default;
  Status NewWritableFile(const std::string& fname,
                         WritableFile** result) override;
  Status NewAppendableFile(const std::string& fname,
                           WritableFile** result) override;
  Status RemoveFile(const std::string& f) override;
  Status RenameFile(const std::string& s, const std::string& t) override;

  void WritableFileClosed(const FileState& state);
  Status DropUnsyncedFileData();
  Status RemoveFilesCreatedAfterLastDirSync();
  void DirWasSynced();
  bool IsFileCreatedSinceLastDirSync(const std::string& filename);
  void ResetState();
  void UntrackFile(const std::string& f);
  // Setting the filesystem to inactive is the test equivalent to simulating a
  // system reset. Setting to inactive will freeze our saved filesystem state so
  // that it will stop being recorded. It can then be reset back to the state at
  // the time of the reset.
  bool IsFilesystemActive() LOCKS_EXCLUDED(mutex_) {
    MutexLock l(&mutex_);
    return filesystem_active_;
  }
  void SetFilesystemActive(bool active) LOCKS_EXCLUDED(mutex_) {
    MutexLock l(&mutex_);
    filesystem_active_ = active;
  }

 private:
  port::Mutex mutex_;
  std::map<std::string, FileState> db_file_state_ GUARDED_BY(mutex_);
  std::set<std::string> new_files_since_last_dir_sync_ GUARDED_BY(mutex_);
  bool filesystem_active_ GUARDED_BY(mutex_);  // Record flushes, syncs, writes
};

TestWritableFile::TestWritableFile(const FileState& state, WritableFile* f,
                                   FaultInjectionTestEnv* env)
    : state_(state), target_(f), writable_file_opened_(true), env_(env) { __builtin_trap() /* STUB: not implemented */; }

TestWritableFile::~TestWritableFile() { __builtin_trap() /* STUB: not implemented */; }

Status TestWritableFile::Append(const Slice& data) { __builtin_trap() /* STUB: not implemented */; }

Status TestWritableFile::Close() { __builtin_trap() /* STUB: not implemented */; }

Status TestWritableFile::Flush() { __builtin_trap() /* STUB: not implemented */; }

Status TestWritableFile::SyncParent() { __builtin_trap() /* STUB: not implemented */; }

Status TestWritableFile::Sync() { __builtin_trap() /* STUB: not implemented */; }

Status FaultInjectionTestEnv::NewWritableFile(const std::string& fname,
                                              WritableFile** result) { __builtin_trap() /* STUB: not implemented */; }

Status FaultInjectionTestEnv::NewAppendableFile(const std::string& fname,
                                                WritableFile** result) { __builtin_trap() /* STUB: not implemented */; }

Status FaultInjectionTestEnv::DropUnsyncedFileData() { __builtin_trap() /* STUB: not implemented */; }

void FaultInjectionTestEnv::DirWasSynced() { __builtin_trap() /* STUB: not implemented */; }

bool FaultInjectionTestEnv::IsFileCreatedSinceLastDirSync(
    const std::string& filename) { __builtin_trap() /* STUB: not implemented */; }

void FaultInjectionTestEnv::UntrackFile(const std::string& f) { __builtin_trap() /* STUB: not implemented */; }

Status FaultInjectionTestEnv::RemoveFile(const std::string& f) { __builtin_trap() /* STUB: not implemented */; }

Status FaultInjectionTestEnv::RenameFile(const std::string& s,
                                         const std::string& t) { __builtin_trap() /* STUB: not implemented */; }

void FaultInjectionTestEnv::ResetState() { __builtin_trap() /* STUB: not implemented */; }

Status FaultInjectionTestEnv::RemoveFilesCreatedAfterLastDirSync() { __builtin_trap() /* STUB: not implemented */; }

void FaultInjectionTestEnv::WritableFileClosed(const FileState& state) { __builtin_trap() /* STUB: not implemented */; }

Status FileState::DropUnsyncedData() const { __builtin_trap() /* STUB: not implemented */; }

class FaultInjectionTest : public testing::Test {
 public:
  enum ExpectedVerifResult { VAL_EXPECT_NO_ERROR, VAL_EXPECT_ERROR };
  enum ResetMethod { RESET_DROP_UNSYNCED_DATA, RESET_DELETE_UNSYNCED_FILES };

  FaultInjectionTestEnv* env_;
  std::string dbname_;
  Cache* tiny_cache_;
  Options options_;
  DB* db_;

  FaultInjectionTest()
      : env_(new FaultInjectionTestEnv),
        tiny_cache_(NewLRUCache(100)),
        db_(nullptr) { __builtin_trap() /* STUB: not implemented */; }

  ~FaultInjectionTest() { __builtin_trap() /* STUB: not implemented */; }

  void ReuseLogs(bool reuse) { __builtin_trap() /* STUB: not implemented */; }

  void Build(int start_idx, int num_vals) { __builtin_trap() /* STUB: not implemented */; }

  Status ReadValue(int i, std::string* val) const { __builtin_trap() /* STUB: not implemented */; }

  Status Verify(int start_idx, int num_vals,
                ExpectedVerifResult expected) const { __builtin_trap() /* STUB: not implemented */; }

  // Return the ith key
  Slice Key(int i, std::string* storage) const { __builtin_trap() /* STUB: not implemented */; }

  // Return the value to associate with the specified key
  Slice Value(int k, std::string* storage) const { __builtin_trap() /* STUB: not implemented */; }

  Status OpenDB() { __builtin_trap() /* STUB: not implemented */; }

  void CloseDB() { __builtin_trap() /* STUB: not implemented */; }

  void DeleteAllData() { __builtin_trap() /* STUB: not implemented */; }

  void ResetDBState(ResetMethod reset_method) { __builtin_trap() /* STUB: not implemented */; }

  void PartialCompactTestPreFault(int num_pre_sync, int num_post_sync) { __builtin_trap() /* STUB: not implemented */; }

  void PartialCompactTestReopenWithFault(ResetMethod reset_method,
                                         int num_pre_sync, int num_post_sync) { __builtin_trap() /* STUB: not implemented */; }

  void NoWriteTestPreFault() { __builtin_trap() /* STUB: not implemented */; }

  void NoWriteTestReopenWithFault(ResetMethod reset_method) { __builtin_trap() /* STUB: not implemented */; }

  void DoTest() { __builtin_trap() /* STUB: not implemented */; }
};

TEST_F(FaultInjectionTest, FaultTestNoLogReuse) {
  ReuseLogs(false);
  DoTest();
}

TEST_F(FaultInjectionTest, FaultTestWithLogReuse) {
  ReuseLogs(true);
  DoTest();
}

}  // namespace leveldb
