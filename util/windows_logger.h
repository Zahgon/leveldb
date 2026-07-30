#include <stdexcept>
#include <cstdlib>
// Copyright (c) 2018 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Logger implementation for the Windows platform.

#ifndef STORAGE_LEVELDB_UTIL_WINDOWS_LOGGER_H_
#define STORAGE_LEVELDB_UTIL_WINDOWS_LOGGER_H_

#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <ctime>
#include <sstream>
#include <thread>

#include "leveldb/env.h"

namespace leveldb {

class WindowsLogger final : public Logger {
 public:
  // Creates a logger that writes to the given file.
  //
  // The PosixLogger instance takes ownership of the file handle.
  explicit WindowsLogger(std::FILE* fp) : fp_(fp) {
    __builtin_trap() /* STUB: not implemented */;
}

  ~WindowsLogger() override { std::fclose(fp_); }

  void Logv(const char* format, std::va_list arguments) override {
    __builtin_trap() /* STUB: not implemented */;
}

 private:
  std::FILE* const fp_;
};

}  // namespace leveldb

#endif  // STORAGE_LEVELDB_UTIL_WINDOWS_LOGGER_H_
