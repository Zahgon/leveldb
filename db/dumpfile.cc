#include <stdexcept>
#include <cstdlib>
// Copyright (c) 2012 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "leveldb/dumpfile.h"

#include <cstdio>

#include "db/dbformat.h"
#include "db/filename.h"
#include "db/log_reader.h"
#include "db/version_edit.h"
#include "db/write_batch_internal.h"
#include "leveldb/env.h"
#include "leveldb/iterator.h"
#include "leveldb/options.h"
#include "leveldb/status.h"
#include "leveldb/table.h"
#include "leveldb/write_batch.h"
#include "util/logging.h"

namespace leveldb {

namespace {

bool GuessType(const std::string& fname, FileType* type) {
    __builtin_trap() /* STUB: not implemented */;
}

// Notified when log reader encounters corruption.
class CorruptionReporter : public log::Reader::Reporter {
 public:
  void Corruption(size_t bytes, const Status& status) override {
    __builtin_trap() /* STUB: not implemented */;
}

  WritableFile* dst_;
};

// Print contents of a log file. (*func)() is called on every record.
Status PrintLogContents(Env* env, const std::string& fname,
                        void (*func)(uint64_t, Slice, WritableFile*),
                        WritableFile* dst) {
    __builtin_trap() /* STUB: not implemented */;
}

// Called on every item found in a WriteBatch.
class WriteBatchItemPrinter : public WriteBatch::Handler {
 public:
  void Put(const Slice& key, const Slice& value) override {
    __builtin_trap() /* STUB: not implemented */;
}
  void Delete(const Slice& key) override {
    __builtin_trap() /* STUB: not implemented */;
}

  WritableFile* dst_;
};

// Called on every log record (each one of which is a WriteBatch)
// found in a kLogFile.
static void WriteBatchPrinter(uint64_t pos, Slice record, WritableFile* dst) {
    __builtin_trap() /* STUB: not implemented */;
}

Status DumpLog(Env* env, const std::string& fname, WritableFile* dst) {
    __builtin_trap() /* STUB: not implemented */;
}

// Called on every log record (each one of which is a WriteBatch)
// found in a kDescriptorFile.
static void VersionEditPrinter(uint64_t pos, Slice record, WritableFile* dst) {
    __builtin_trap() /* STUB: not implemented */;
}

Status DumpDescriptor(Env* env, const std::string& fname, WritableFile* dst) {
    __builtin_trap() /* STUB: not implemented */;
}

Status DumpTable(Env* env, const std::string& fname, WritableFile* dst) {
    __builtin_trap() /* STUB: not implemented */;
}

}  // namespace

Status DumpFile(Env* env, const std::string& fname, WritableFile* dst) {
    __builtin_trap() /* STUB: not implemented */;
}

}  // namespace leveldb
