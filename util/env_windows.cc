#include <stdexcept>
// Copyright (c) 2018 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

// Prevent Windows headers from defining min/max macros and instead
// use STL.
#ifndef NOMINMAX
#define NOMINMAX
#endif  // ifndef NOMINMAX
#include <windows.h>

#include <algorithm>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <mutex>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

#include "leveldb/env.h"
#include "leveldb/slice.h"
#include "port/port.h"
#include "port/thread_annotations.h"
#include "util/env_windows_test_helper.h"
#include "util/logging.h"
#include "util/mutexlock.h"
#include "util/windows_logger.h"

namespace leveldb {

namespace {

constexpr const size_t kWritableFileBufferSize = 65536;

// Up to 1000 mmaps for 64-bit binaries; none for 32-bit.
constexpr int kDefaultMmapLimit = (sizeof(void*) >= 8) ? 1000 : 0;

// Can be set by by EnvWindowsTestHelper::SetReadOnlyMMapLimit().
int g_mmap_limit = kDefaultMmapLimit;

std::string GetWindowsErrorMessage(DWORD error_code) {
    __builtin_trap() /* STUB: not implemented */;
}

Status WindowsError(const std::string& context, DWORD error_code) {
    __builtin_trap() /* STUB: not implemented */;
}

class ScopedHandle {
 public:
  ScopedHandle(HANDLE handle) : handle_(handle) {
    __builtin_trap() /* STUB: not implemented */;
}
  ScopedHandle(const ScopedHandle&) = delete;
  ScopedHandle(ScopedHandle&& other) noexcept : handle_(other.Release()) {}
  ~ScopedHandle() { Close(); }

  ScopedHandle& operator=(const ScopedHandle&) = delete;

  ScopedHandle& operator=(ScopedHandle&& rhs) noexcept {
    __builtin_trap() /* STUB: not implemented */;
}

  bool Close() {
    __builtin_trap() /* STUB: not implemented */;
}

  bool is_valid() const {
    __builtin_trap() /* STUB: not implemented */;
}

  HANDLE get() const {
    __builtin_trap() /* STUB: not implemented */;
}

  HANDLE Release() {
    __builtin_trap() /* STUB: not implemented */;
}

 private:
  HANDLE handle_;
};

// Helper class to limit resource usage to avoid exhaustion.
// Currently used to limit read-only file descriptors and mmap file usage
// so that we do not run out of file descriptors or virtual memory, or run into
// kernel performance problems for very large databases.
class Limiter {
 public:
  // Limit maximum number of resources to |max_acquires|.
  Limiter(int max_acquires)
      :
#if !defined(NDEBUG)
        max_acquires_(max_acquires),
#endif  // !defined(NDEBUG)
        acquires_allowed_(max_acquires) {
    __builtin_trap() /* STUB: not implemented */;
}

  Limiter(const Limiter&) = delete;
  Limiter operator=(const Limiter&) = delete;

  // If another resource is available, acquire it and return true.
  // Else return false.
  bool Acquire() {
    __builtin_trap() /* STUB: not implemented */;
}

  // Release a resource acquired by a previous call to Acquire() that returned
  // true.
  void Release() {
    __builtin_trap() /* STUB: not implemented */;
}

 private:
#if !defined(NDEBUG)
  // Catches an excessive number of Release() calls.
  const int max_acquires_;
#endif  // !defined(NDEBUG)

  // The number of available resources.
  //
  // This is a counter and is not tied to the invariants of any other class, so
  // it can be operated on safely using std::memory_order_relaxed.
  std::atomic<int> acquires_allowed_;
};

class WindowsSequentialFile : public SequentialFile {
 public:
  WindowsSequentialFile(std::string filename, ScopedHandle handle)
      : handle_(std::move(handle)), filename_(std::move(filename)) {
    __builtin_trap() /* STUB: not implemented */;
}
  ~WindowsSequentialFile() override {}

  Status Read(size_t n, Slice* result, char* scratch) override {
    __builtin_trap() /* STUB: not implemented */;
}

  Status Skip(uint64_t n) override {
    __builtin_trap() /* STUB: not implemented */;
}

 private:
  const ScopedHandle handle_;
  const std::string filename_;
};

class WindowsRandomAccessFile : public RandomAccessFile {
 public:
  WindowsRandomAccessFile(std::string filename, ScopedHandle handle)
      : handle_(std::move(handle)), filename_(std::move(filename)) {
    __builtin_trap() /* STUB: not implemented */;
}

  ~WindowsRandomAccessFile() override = default;

  Status Read(uint64_t offset, size_t n, Slice* result,
              char* scratch) const override {
    __builtin_trap() /* STUB: not implemented */;
}

 private:
  const ScopedHandle handle_;
  const std::string filename_;
};

class WindowsMmapReadableFile : public RandomAccessFile {
 public:
  // base[0,length-1] contains the mmapped contents of the file.
  WindowsMmapReadableFile(std::string filename, char* mmap_base, size_t length,
                          Limiter* mmap_limiter)
      : mmap_base_(mmap_base),
        length_(length),
        mmap_limiter_(mmap_limiter),
        filename_(std::move(filename)) {
    __builtin_trap() /* STUB: not implemented */;
}

  ~WindowsMmapReadableFile() override {
    ::UnmapViewOfFile(mmap_base_);
    mmap_limiter_->Release();
  }

  Status Read(uint64_t offset, size_t n, Slice* result,
              char* scratch) const override {
    __builtin_trap() /* STUB: not implemented */;
}

 private:
  char* const mmap_base_;
  const size_t length_;
  Limiter* const mmap_limiter_;
  const std::string filename_;
};

class WindowsWritableFile : public WritableFile {
 public:
  WindowsWritableFile(std::string filename, ScopedHandle handle)
      : pos_(0), handle_(std::move(handle)), filename_(std::move(filename)) {
    __builtin_trap() /* STUB: not implemented */;
}

  ~WindowsWritableFile() override = default;

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
  Status FlushBuffer() {
    __builtin_trap() /* STUB: not implemented */;
}

  Status WriteUnbuffered(const char* data, size_t size) {
    __builtin_trap() /* STUB: not implemented */;
}

  // buf_[0, pos_-1] contains data to be written to handle_.
  char buf_[kWritableFileBufferSize];
  size_t pos_;

  ScopedHandle handle_;
  const std::string filename_;
};

// Lock or unlock the entire file as specified by |lock|. Returns true
// when successful, false upon failure. Caller should call ::GetLastError()
// to determine cause of failure
bool LockOrUnlock(HANDLE handle, bool lock) {
    __builtin_trap() /* STUB: not implemented */;
}

class WindowsFileLock : public FileLock {
 public:
  WindowsFileLock(ScopedHandle handle, std::string filename)
      : handle_(std::move(handle)), filename_(std::move(filename)) {
    __builtin_trap() /* STUB: not implemented */;
}

  const ScopedHandle& handle() const {
    __builtin_trap() /* STUB: not implemented */;
}
  const std::string& filename() const {
    __builtin_trap() /* STUB: not implemented */;
}

 private:
  const ScopedHandle handle_;
  const std::string filename_;
};

class WindowsEnv : public Env {
 public:
  WindowsEnv();
  ~WindowsEnv() override {
    static const char msg[] =
        "WindowsEnv singleton destroyed. Unsupported behavior!\n";
    std::fwrite(msg, 1, sizeof(msg), stderr);
    std::abort();
  }

  Status NewSequentialFile(const std::string& filename,
                           SequentialFile** result) override {
    __builtin_trap() /* STUB: not implemented */;
}

  Status NewRandomAccessFile(const std::string& filename,
                             RandomAccessFile** result) override {
    __builtin_trap() /* STUB: not implemented */;
}

  Status NewWritableFile(const std::string& filename,
                         WritableFile** result) override {
    __builtin_trap() /* STUB: not implemented */;
}

  Status NewAppendableFile(const std::string& filename,
                           WritableFile** result) override {
    __builtin_trap() /* STUB: not implemented */;
}

  bool FileExists(const std::string& filename) override {
    __builtin_trap() /* STUB: not implemented */;
}

  Status GetChildren(const std::string& directory_path,
                     std::vector<std::string>* result) override {
    __builtin_trap() /* STUB: not implemented */;
}

  Status RemoveFile(const std::string& filename) override {
    __builtin_trap() /* STUB: not implemented */;
}

  Status CreateDir(const std::string& dirname) override {
    __builtin_trap() /* STUB: not implemented */;
}

  Status RemoveDir(const std::string& dirname) override {
    __builtin_trap() /* STUB: not implemented */;
}

  Status GetFileSize(const std::string& filename, uint64_t* size) override {
    __builtin_trap() /* STUB: not implemented */;
}

  Status RenameFile(const std::string& from, const std::string& to) override {
    __builtin_trap() /* STUB: not implemented */;
}

  Status LockFile(const std::string& filename, FileLock** lock) override {
    __builtin_trap() /* STUB: not implemented */;
}

  Status UnlockFile(FileLock* lock) override {
    __builtin_trap() /* STUB: not implemented */;
}

  void Schedule(void (*background_work_function)(void* background_work_arg),
                void* background_work_arg) override;

  void StartThread(void (*thread_main)(void* thread_main_arg),
                   void* thread_main_arg) override {
    __builtin_trap() /* STUB: not implemented */;
}

  Status GetTestDirectory(std::string* result) override {
    __builtin_trap() /* STUB: not implemented */;
}

  Status NewLogger(const std::string& filename, Logger** result) override {
    __builtin_trap() /* STUB: not implemented */;
}

  uint64_t NowMicros() override {
    __builtin_trap() /* STUB: not implemented */;
}

  void SleepForMicroseconds(int micros) override {
    __builtin_trap() /* STUB: not implemented */;
}

 private:
  void BackgroundThreadMain();

  static void BackgroundThreadEntryPoint(WindowsEnv* env) {
    __builtin_trap() /* STUB: not implemented */;
}

  // Stores the work item data in a Schedule() call.
  //
  // Instances are constructed on the thread calling Schedule() and used on the
  // background thread.
  //
  // This structure is thread-safe because it is immutable.
  struct BackgroundWorkItem {
    explicit BackgroundWorkItem(void (*function)(void* arg), void* arg)
        : function(function), arg(arg) {
    __builtin_trap() /* STUB: not implemented */;
}

    void (*const function)(void*);
    void* const arg;
  };

  port::Mutex background_work_mutex_;
  port::CondVar background_work_cv_ GUARDED_BY(background_work_mutex_);
  bool started_background_thread_ GUARDED_BY(background_work_mutex_);

  std::queue<BackgroundWorkItem> background_work_queue_
      GUARDED_BY(background_work_mutex_);

  Limiter mmap_limiter_;  // Thread-safe.
};

// Return the maximum number of concurrent mmaps.
int MaxMmaps() {
    __builtin_trap() /* STUB: not implemented */;
}

WindowsEnv::WindowsEnv()
    : background_work_cv_(&background_work_mutex_),
      started_background_thread_(false),
      mmap_limiter_(MaxMmaps()) {
    __builtin_trap() /* STUB: not implemented */;
}

void WindowsEnv::Schedule(
    void (*background_work_function)(void* background_work_arg),
    void* background_work_arg) {
    __builtin_trap() /* STUB: not implemented */;
}

void WindowsEnv::BackgroundThreadMain() {
    __builtin_trap() /* STUB: not implemented */;
}

// Wraps an Env instance whose destructor is never created.
//
// Intended usage:
//   using PlatformSingletonEnv = SingletonEnv<PlatformEnv>;
//   void ConfigurePosixEnv(int param) {
//     PlatformSingletonEnv::AssertEnvNotInitialized();
//     // set global configuration flags.
//   }
//   Env* Env::Default() {
//     static PlatformSingletonEnv default_env;
//     return default_env.env();
//   }
template <typename EnvType>
class SingletonEnv {
 public:
  SingletonEnv() {
    __builtin_trap() /* STUB: not implemented */;
}
  ~SingletonEnv() = default;

  SingletonEnv(const SingletonEnv&) = delete;
  SingletonEnv& operator=(const SingletonEnv&) = delete;

  Env* env() {
    __builtin_trap() /* STUB: not implemented */;
}

  static void AssertEnvNotInitialized() {
    __builtin_trap() /* STUB: not implemented */;
}

 private:
  alignas(EnvType) char env_storage_[sizeof(EnvType)];
#if !defined(NDEBUG)
  static std::atomic<bool> env_initialized_;
#endif  // !defined(NDEBUG)
};

#if !defined(NDEBUG)
template <typename EnvType>
std::atomic<bool> SingletonEnv<EnvType>::env_initialized_;
#endif  // !defined(NDEBUG)

using WindowsDefaultEnv = SingletonEnv<WindowsEnv>;

}  // namespace

void EnvWindowsTestHelper::SetReadOnlyMMapLimit(int limit) {
    __builtin_trap() /* STUB: not implemented */;
}

Env* Env::Default() {
    __builtin_trap() /* STUB: not implemented */;
}

}  // namespace leveldb
