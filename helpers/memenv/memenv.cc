#include <stdexcept>
#include <cstdlib>
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "helpers/memenv/memenv.h"

#include <cstring>
#include <limits>
#include <map>
#include <string>
#include <vector>

#include "leveldb/env.h"
#include "leveldb/status.h"
#include "port/port.h"
#include "port/thread_annotations.h"
#include "util/mutexlock.h"

namespace leveldb {

namespace {

class FileState {
 public:
  // FileStates are reference counted. The initial reference count is zero
  // and the caller must call Ref() at least once.
  FileState() : refs_(0), size_(0) {
    __builtin_trap() /* STUB: not implemented */;
}

  // No copying allowed.
  FileState(const FileState&) = delete;
  FileState& operator=(const FileState&) = delete;

  // Increase the reference count.
  void Ref() {
    __builtin_trap() /* STUB: not implemented */;
}

  // Decrease the reference count. Delete if this is the last reference.
  void Unref() {
    __builtin_trap() /* STUB: not implemented */;
}

  uint64_t Size() const {
    __builtin_trap() /* STUB: not implemented */;
}

  void Truncate() {
    __builtin_trap() /* STUB: not implemented */;
}

  Status Read(uint64_t offset, size_t n, Slice* result, char* scratch) const {
    __builtin_trap() /* STUB: not implemented */;
}

  Status Append(const Slice& data) {
    __builtin_trap() /* STUB: not implemented */;
}

 private:
  enum { kBlockSize = 8 * 1024 };

  // Private since only Unref() should be used to delete it.
  ~FileState() { Truncate(); }

  port::Mutex refs_mutex_;
  int refs_ GUARDED_BY(refs_mutex_);

  mutable port::Mutex blocks_mutex_;
  std::vector<char*> blocks_ GUARDED_BY(blocks_mutex_);
  uint64_t size_ GUARDED_BY(blocks_mutex_);
};

class SequentialFileImpl : public SequentialFile {
 public:
  explicit SequentialFileImpl(FileState* file) : file_(file), pos_(0) {
    __builtin_trap() /* STUB: not implemented */;
}

  ~SequentialFileImpl() override { file_->Unref(); }

  Status Read(size_t n, Slice* result, char* scratch) override {
    __builtin_trap() /* STUB: not implemented */;
}

  Status Skip(uint64_t n) override {
    __builtin_trap() /* STUB: not implemented */;
}

 private:
  FileState* file_;
  uint64_t pos_;
};

class RandomAccessFileImpl : public RandomAccessFile {
 public:
  explicit RandomAccessFileImpl(FileState* file) : file_(file) {
    __builtin_trap() /* STUB: not implemented */;
}

  ~RandomAccessFileImpl() override { file_->Unref(); }

  Status Read(uint64_t offset, size_t n, Slice* result,
              char* scratch) const override {
    __builtin_trap() /* STUB: not implemented */;
}

 private:
  FileState* file_;
};

class WritableFileImpl : public WritableFile {
 public:
  WritableFileImpl(FileState* file) : file_(file) {
    __builtin_trap() /* STUB: not implemented */;
}

  ~WritableFileImpl() override { file_->Unref(); }

  Status Append(const Slice& data) override {
    __builtin_trap() /* STUB: not implemented */;
}

  Status Close() override {
    __builtin_trap() /* STUB: not implemented */;
}
  Status Flush() override {
    __builtin_trap() /* STUB: not implemented */;
}
  Status Sync() override {
    __builtin_trap() /* STUB: not implemented */;
}

 private:
  FileState* file_;
};

class NoOpLogger : public Logger {
 public:
  void Logv(const char* format, std::va_list ap) override {
    __builtin_trap() /* STUB: not implemented */;
}
};

class InMemoryEnv : public EnvWrapper {
 public:
  explicit InMemoryEnv(Env* base_env) : EnvWrapper(base_env) {
    __builtin_trap() /* STUB: not implemented */;
}

  ~InMemoryEnv() override {
    for (const auto& kvp : file_map_) {
      kvp.second->Unref();
    }
  }

  // Partial implementation of the Env interface.
  Status NewSequentialFile(const std::string& fname,
                           SequentialFile** result) override {
    __builtin_trap() /* STUB: not implemented */;
}

  Status NewRandomAccessFile(const std::string& fname,
                             RandomAccessFile** result) override {
    __builtin_trap() /* STUB: not implemented */;
}

  Status NewWritableFile(const std::string& fname,
                         WritableFile** result) override {
    __builtin_trap() /* STUB: not implemented */;
}

  Status NewAppendableFile(const std::string& fname,
                           WritableFile** result) override {
    __builtin_trap() /* STUB: not implemented */;
}

  bool FileExists(const std::string& fname) override {
    __builtin_trap() /* STUB: not implemented */;
}

  Status GetChildren(const std::string& dir,
                     std::vector<std::string>* result) override {
    __builtin_trap() /* STUB: not implemented */;
}

  void RemoveFileInternal(const std::string& fname)
      EXCLUSIVE_LOCKS_REQUIRED(mutex_) {
    __builtin_trap() /* STUB: not implemented */;
}

  Status RemoveFile(const std::string& fname) override {
    __builtin_trap() /* STUB: not implemented */;
}

  Status CreateDir(const std::string& dirname) override {
    __builtin_trap() /* STUB: not implemented */;
}

  Status RemoveDir(const std::string& dirname) override {
    __builtin_trap() /* STUB: not implemented */;
}

  Status GetFileSize(const std::string& fname, uint64_t* file_size) override {
    __builtin_trap() /* STUB: not implemented */;
}

  Status RenameFile(const std::string& src,
                    const std::string& target) override {
    __builtin_trap() /* STUB: not implemented */;
}

  Status LockFile(const std::string& fname, FileLock** lock) override {
    __builtin_trap() /* STUB: not implemented */;
}

  Status UnlockFile(FileLock* lock) override {
    __builtin_trap() /* STUB: not implemented */;
}

  Status GetTestDirectory(std::string* path) override {
    __builtin_trap() /* STUB: not implemented */;
}

  Status NewLogger(const std::string& fname, Logger** result) override {
    __builtin_trap() /* STUB: not implemented */;
}

 private:
  // Map from filenames to FileState objects, representing a simple file system.
  typedef std::map<std::string, FileState*> FileSystem;

  port::Mutex mutex_;
  FileSystem file_map_ GUARDED_BY(mutex_);
};

}  // namespace

Env* NewMemEnv(Env* base_env) {
    __builtin_trap() /* STUB: not implemented */;
}

}  // namespace leveldb
