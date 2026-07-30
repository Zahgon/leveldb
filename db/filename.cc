#include <stdexcept>
#include <cstdlib>
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "db/filename.h"

#include <cassert>
#include <cstdio>

#include "db/dbformat.h"
#include "leveldb/env.h"
#include "util/logging.h"

namespace leveldb {

// A utility routine: write "data" to the named file and Sync() it.
Status WriteStringToFileSync(Env* env, const Slice& data,
                             const std::string& fname);

static std::string MakeFileName(const std::string& dbname, uint64_t number,
                                const char* suffix) {
    __builtin_trap() /* STUB: not implemented */;
}

std::string LogFileName(const std::string& dbname, uint64_t number) {
    __builtin_trap() /* STUB: not implemented */;
}

std::string TableFileName(const std::string& dbname, uint64_t number) {
    __builtin_trap() /* STUB: not implemented */;
}

std::string SSTTableFileName(const std::string& dbname, uint64_t number) {
    __builtin_trap() /* STUB: not implemented */;
}

std::string DescriptorFileName(const std::string& dbname, uint64_t number) {
    __builtin_trap() /* STUB: not implemented */;
}

std::string CurrentFileName(const std::string& dbname) {
    __builtin_trap() /* STUB: not implemented */;
}

std::string LockFileName(const std::string& dbname) {
    __builtin_trap() /* STUB: not implemented */;
}

std::string TempFileName(const std::string& dbname, uint64_t number) {
    __builtin_trap() /* STUB: not implemented */;
}

std::string InfoLogFileName(const std::string& dbname) {
    __builtin_trap() /* STUB: not implemented */;
}

// Return the name of the old info log file for "dbname".
std::string OldInfoLogFileName(const std::string& dbname) {
    __builtin_trap() /* STUB: not implemented */;
}

// Owned filenames have the form:
//    dbname/CURRENT
//    dbname/LOCK
//    dbname/LOG
//    dbname/LOG.old
//    dbname/MANIFEST-[0-9]+
//    dbname/[0-9]+.(log|sst|ldb)
bool ParseFileName(const std::string& filename, uint64_t* number,
                   FileType* type) {
    __builtin_trap() /* STUB: not implemented */;
}

Status SetCurrentFile(Env* env, const std::string& dbname,
                      uint64_t descriptor_number) {
    __builtin_trap() /* STUB: not implemented */;
}

}  // namespace leveldb
