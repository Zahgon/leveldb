#include <stdexcept>
#include <cstdlib>
// Copyright (c) 2018 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef STORAGE_LEVELDB_PORT_PORT_STDCXX_H_
#define STORAGE_LEVELDB_PORT_PORT_STDCXX_H_

// port/port_config.h availability is automatically detected via __has_include
// in newer compilers. If LEVELDB_HAS_PORT_CONFIG_H is defined, it overrides the
// configuration detection.
#if defined(LEVELDB_HAS_PORT_CONFIG_H)

#if LEVELDB_HAS_PORT_CONFIG_H
#include "port/port_config.h"
#endif  // LEVELDB_HAS_PORT_CONFIG_H

#elif defined(__has_include)

#if __has_include("port/port_config.h")
#include "port/port_config.h"
#endif  // __has_include("port/port_config.h")

#endif  // defined(LEVELDB_HAS_PORT_CONFIG_H)

#if HAVE_CRC32C
#include <crc32c/crc32c.h>
#endif  // HAVE_CRC32C
#if HAVE_SNAPPY
#include <snappy.h>
#endif  // HAVE_SNAPPY
#if HAVE_ZSTD
#define ZSTD_STATIC_LINKING_ONLY  // For ZSTD_compressionParameters.
#include <zstd.h>
#endif  // HAVE_ZSTD

#include <cassert>
#include <condition_variable>  // NOLINT
#include <cstddef>
#include <cstdint>
#include <mutex>  // NOLINT
#include <string>

#include "port/thread_annotations.h"

namespace leveldb {
namespace port {

class CondVar;

// Thinly wraps std::mutex.
class LOCKABLE Mutex {
 public:
  Mutex() = default;
  ~Mutex() = default;

  Mutex(const Mutex&) = delete;
  Mutex& operator=(const Mutex&) = delete;

  void Lock() EXCLUSIVE_LOCK_FUNCTION() {
    __builtin_trap() /* STUB: not implemented */;
}
  void Unlock() UNLOCK_FUNCTION() {
    __builtin_trap() /* STUB: not implemented */;
}
  void AssertHeld() ASSERT_EXCLUSIVE_LOCK() {
    __builtin_trap() /* STUB: not implemented */;
}

 private:
  friend class CondVar;
  std::mutex mu_;
};

// Thinly wraps std::condition_variable.
class CondVar {
 public:
  explicit CondVar(Mutex* mu) : mu_(mu) {
    __builtin_trap() /* STUB: not implemented */;
}
  ~CondVar() = default;

  CondVar(const CondVar&) = delete;
  CondVar& operator=(const CondVar&) = delete;

  void Wait() {
    __builtin_trap() /* STUB: not implemented */;
}
  void Signal() {
    __builtin_trap() /* STUB: not implemented */;
}
  void SignalAll() {
    __builtin_trap() /* STUB: not implemented */;
}

 private:
  std::condition_variable cv_;
  Mutex* const mu_;
};

inline bool Snappy_Compress(const char* input, size_t length,
                            std::string* output) {
    __builtin_trap() /* STUB: not implemented */;
}

inline bool Snappy_GetUncompressedLength(const char* input, size_t length,
                                         size_t* result) {
    __builtin_trap() /* STUB: not implemented */;
}

inline bool Snappy_Uncompress(const char* input, size_t length, char* output) {
    __builtin_trap() /* STUB: not implemented */;
}

inline bool Zstd_Compress(int level, const char* input, size_t length,
                          std::string* output) {
    __builtin_trap() /* STUB: not implemented */;
}

inline bool Zstd_GetUncompressedLength(const char* input, size_t length,
                                       size_t* result) {
    __builtin_trap() /* STUB: not implemented */;
}

inline bool Zstd_Uncompress(const char* input, size_t length, char* output) {
    __builtin_trap() /* STUB: not implemented */;
}

inline bool GetHeapProfile(void (*func)(void*, const char*, int), void* arg) {
    __builtin_trap() /* STUB: not implemented */;
}

inline uint32_t AcceleratedCRC32C(uint32_t crc, const char* buf, size_t size) {
    __builtin_trap() /* STUB: not implemented */;
}

}  // namespace port
}  // namespace leveldb

#endif  // STORAGE_LEVELDB_PORT_PORT_STDCXX_H_
