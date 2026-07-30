#include <stdexcept>
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include <dirent.h>
#include <fcntl.h>
#include <sys/mman.h>
#ifndef __Fuchsia__
#include <sys/resource.h>
#endif
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <atomic>
#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <queue>
#include <set>
#include <string>
#include <thread>
#include <type_traits>
#include <utility>

#include "leveldb/env.h"
#include "leveldb/slice.h"
#include "leveldb/status.h"
#include "port/port.h"
#include "port/thread_annotations.h"
#include "util/env_posix_test_helper.h"
#include "util/posix_logger.h"

namespace leveldb {

namespace {

// Set by EnvPosixTestHelper::SetReadOnlyMMapLimit() and MaxOpenFiles().
int g_open_read_only_file_limit = -1;

// Up to 1000 mmap regions for 64-bit binaries; none for 32-bit.
constexpr const int kDefaultMmapLimit = (sizeof(void*) >= 8) ? 1000 : 0;

// Can be set using EnvPosixTestHelper::SetReadOnlyMMapLimit().
int g_mmap_limit = kDefaultMmapLimit;

// Common flags defined for all posix open operations
#if defined(HAVE_O_CLOEXEC)
constexpr const int kOpenBaseFlags = O_CLOEXEC;
#else
constexpr const int kOpenBaseFlags = 0;
#endif  // defined(HAVE_O_CLOEXEC)

constexpr const size_t kWritableFileBufferSize = 65536;

Status PosixError(const std::string& context, int error_number) {
    __builtin_trap() /* STUB: not implemented */;
}

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

// Implements sequential read access in a file using read().
//
// Instances of this class are thread-friendly but not thread-safe, as required
// by the SequentialFile API.
class PosixSequentialFile final : public SequentialFile {
 public:
  PosixSequentialFile(std::string filename, int fd)
      : fd_(fd), filename_(std::move(filename)) {
    __builtin_trap() /* STUB: not implemented */;
}
  ~PosixSequentialFile() override { close(fd_); }

  Status Read(size_t n, Slice* result, char* scratch) override {
    __builtin_trap() /* STUB: not implemented */;
}

  Status Skip(uint64_t n) override {
    __builtin_trap() /* STUB: not implemented */;
}

 private:
  const int fd_;
  const std::string filename_;
};

// Implements random read access in a file using pread().
//
// Instances of this class are thread-safe, as required by the RandomAccessFile
// API. Instances are immutable and Read() only calls thread-safe library
// functions.
class PosixRandomAccessFile final : public RandomAccessFile {
 public:
  // The new instance takes ownership of |fd|. |fd_limiter| must outlive this
  // instance, and will be used to determine if .
  PosixRandomAccessFile(std::string filename, int fd, Limiter* fd_limiter)
      : has_permanent_fd_(fd_limiter->Acquire()),
        fd_(has_permanent_fd_ ? fd : -1),
        fd_limiter_(fd_limiter),
        filename_(std::move(filename)) {
    __builtin_trap() /* STUB: not implemented */;
}

  ~PosixRandomAccessFile() override {
    if (has_permanent_fd_) {
      assert(fd_ != -1);
      ::close(fd_);
      fd_limiter_->Release();
    }
  }

  Status Read(uint64_t offset, size_t n, Slice* result,
              char* scratch) const override {
    __builtin_trap() /* STUB: not implemented */;
}

 private:
  const bool has_permanent_fd_;  // If false, the file is opened on every read.
  const int fd_;                 // -1 if has_permanent_fd_ is false.
  Limiter* const fd_limiter_;
  const std::string filename_;
};

// Implements random read access in a file using mmap().
//
// Instances of this class are thread-safe, as required by the RandomAccessFile
// API. Instances are immutable and Read() only calls thread-safe library
// functions.
class PosixMmapReadableFile final : public RandomAccessFile {
 public:
  // mmap_base[0, length-1] points to the memory-mapped contents of the file. It
  // must be the result of a successful call to mmap(). This instances takes
  // over the ownership of the region.
  //
  // |mmap_limiter| must outlive this instance. The caller must have already
  // acquired the right to use one mmap region, which will be released when this
  // instance is destroyed.
  PosixMmapReadableFile(std::string filename, char* mmap_base, size_t length,
                        Limiter* mmap_limiter)
      : mmap_base_(mmap_base),
        length_(length),
        mmap_limiter_(mmap_limiter),
        filename_(std::move(filename)) {
    __builtin_trap() /* STUB: not implemented */;
}

  ~PosixMmapReadableFile() override {
    ::munmap(static_cast<void*>(mmap_base_), length_);
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

class PosixWritableFile final : public WritableFile {
 public:
  PosixWritableFile(std::string filename, int fd)
      : pos_(0),
        fd_(fd),
        is_manifest_(IsManifest(filename)),
        filename_(std::move(filename)),
        dirname_(Dirname(filename_)) {
    __builtin_trap() /* STUB: not implemented */;
}

  ~PosixWritableFile() override {
    if (fd_ >= 0) {
      // Ignoring any potential errors
      Close();
    }
  }

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

  Status SyncDirIfManifest() {
    __builtin_trap() /* STUB: not implemented */;
}

  // Ensures that all the caches associated with the given file descriptor's
  // data are flushed all the way to durable media, and can withstand power
  // failures.
  //
  // The path argument is only used to populate the description string in the
  // returned Status if an error occurs.
  static Status SyncFd(int fd, const std::string& fd_path) {
    __builtin_trap() /* STUB: not implemented */;
}

  // Returns the directory name in a path pointing to a file.
  //
  // Returns "." if the path does not contain any directory separator.
  static std::string Dirname(const std::string& filename) {
    __builtin_trap() /* STUB: not implemented */;
}

  // Extracts the file name from a path pointing to a file.
  //
  // The returned Slice points to |filename|'s data buffer, so it is only valid
  // while |filename| is alive and unchanged.
  static Slice Basename(const std::string& filename) {
    __builtin_trap() /* STUB: not implemented */;
}

  // True if the given file is a manifest file.
  static bool IsManifest(const std::string& filename) {
    __builtin_trap() /* STUB: not implemented */;
}

  // buf_[0, pos_ - 1] contains data to be written to fd_.
  char buf_[kWritableFileBufferSize];
  size_t pos_;
  int fd_;

  const bool is_manifest_;  // True if the file's name starts with MANIFEST.
  const std::string filename_;
  const std::string dirname_;  // The directory of filename_.
};

int LockOrUnlock(int fd, bool lock) {
    __builtin_trap() /* STUB: not implemented */;
}

// Instances are thread-safe because they are immutable.
class PosixFileLock : public FileLock {
 public:
  PosixFileLock(int fd, std::string filename)
      : fd_(fd), filename_(std::move(filename)) {
    __builtin_trap() /* STUB: not implemented */;
}

  int fd() const {
    __builtin_trap() /* STUB: not implemented */;
}
  const std::string& filename() const {
    __builtin_trap() /* STUB: not implemented */;
}

 private:
  const int fd_;
  const std::string filename_;
};

// Tracks the files locked by PosixEnv::LockFile().
//
// We maintain a separate set instead of relying on fcntl(F_SETLK) because
// fcntl(F_SETLK) does not provide any protection against multiple uses from the
// same process.
//
// Instances are thread-safe because all member data is guarded by a mutex.
class PosixLockTable {
 public:
  bool Insert(const std::string& fname) LOCKS_EXCLUDED(mu_) {
    __builtin_trap() /* STUB: not implemented */;
}
  void Remove(const std::string& fname) LOCKS_EXCLUDED(mu_) {
    __builtin_trap() /* STUB: not implemented */;
}

 private:
  port::Mutex mu_;
  std::set<std::string> locked_files_ GUARDED_BY(mu_);
};

class PosixEnv : public Env {
 public:
  PosixEnv();
  ~PosixEnv() override {
    static const char msg[] =
        "PosixEnv singleton destroyed. Unsupported behavior!\n";
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

  static void BackgroundThreadEntryPoint(PosixEnv* env) {
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

  PosixLockTable locks_;  // Thread-safe.
  Limiter mmap_limiter_;  // Thread-safe.
  Limiter fd_limiter_;    // Thread-safe.
};

// Return the maximum number of concurrent mmaps.
int MaxMmaps() {
    __builtin_trap() /* STUB: not implemented */;
}

// Return the maximum number of read-only files to keep open.
int MaxOpenFiles() {
    __builtin_trap() /* STUB: not implemented */;
}

}  // namespace

PosixEnv::PosixEnv()
    : background_work_cv_(&background_work_mutex_),
      started_background_thread_(false),
      mmap_limiter_(MaxMmaps()),
      fd_limiter_(MaxOpenFiles()) {
    __builtin_trap() /* STUB: not implemented */;
}

void PosixEnv::Schedule(
    void (*background_work_function)(void* background_work_arg),
    void* background_work_arg) {
    __builtin_trap() /* STUB: not implemented */;
}

void PosixEnv::BackgroundThreadMain() {
    __builtin_trap() /* STUB: not implemented */;
}

namespace {

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

using PosixDefaultEnv = SingletonEnv<PosixEnv>;

}  // namespace

void EnvPosixTestHelper::SetReadOnlyFDLimit(int limit) {
    __builtin_trap() /* STUB: not implemented */;
}

void EnvPosixTestHelper::SetReadOnlyMMapLimit(int limit) {
    __builtin_trap() /* STUB: not implemented */;
}

Env* Env::Default() {
    __builtin_trap() /* STUB: not implemented */;
}

}  // namespace leveldb
