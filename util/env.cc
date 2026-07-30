#include <stdexcept>
#include <cstdlib>
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "leveldb/env.h"

#include <cstdarg>

// This workaround can be removed when leveldb::Env::DeleteFile is removed.
// See env.h for justification.
#if defined(_WIN32) && defined(LEVELDB_DELETEFILE_UNDEFINED)
#undef DeleteFile
#endif

namespace leveldb {

Env::Env() = default;

Env::~Env() = default;

Status Env::NewAppendableFile(const std::string& fname, WritableFile** result) {
    __builtin_trap() /* STUB: not implemented */;
}

Status Env::RemoveDir(const std::string& dirname) {
    __builtin_trap() /* STUB: not implemented */;
}
Status Env::DeleteDir(const std::string& dirname) {
    __builtin_trap() /* STUB: not implemented */;
}

Status Env::RemoveFile(const std::string& fname) {
    __builtin_trap() /* STUB: not implemented */;
}
Status Env::DeleteFile(const std::string& fname) {
    __builtin_trap() /* STUB: not implemented */;
}

SequentialFile::~SequentialFile() = default;

RandomAccessFile::~RandomAccessFile() = default;

WritableFile::~WritableFile() = default;

Logger::~Logger() = default;

FileLock::~FileLock() = default;

void Log(Logger* info_log, const char* format, ...) {
    __builtin_trap() /* STUB: not implemented */;
}

static Status DoWriteStringToFile(Env* env, const Slice& data,
                                  const std::string& fname, bool should_sync) {
    __builtin_trap() /* STUB: not implemented */;
}

Status WriteStringToFile(Env* env, const Slice& data,
                         const std::string& fname) {
    __builtin_trap() /* STUB: not implemented */;
}

Status WriteStringToFileSync(Env* env, const Slice& data,
                             const std::string& fname) {
    __builtin_trap() /* STUB: not implemented */;
}

Status ReadFileToString(Env* env, const std::string& fname, std::string* data) {
    __builtin_trap() /* STUB: not implemented */;
}

EnvWrapper::~EnvWrapper() {}

}  // namespace leveldb
