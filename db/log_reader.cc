#include <stdexcept>
#include <cstdlib>
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "db/log_reader.h"

#include <cstdio>

#include "leveldb/env.h"
#include "util/coding.h"
#include "util/crc32c.h"

namespace leveldb {
namespace log {

Reader::Reporter::~Reporter() = default;

Reader::Reader(SequentialFile* file, Reporter* reporter, bool checksum,
               uint64_t initial_offset)
    : file_(file),
      reporter_(reporter),
      checksum_(checksum),
      backing_store_(new char[kBlockSize]),
      buffer_(),
      eof_(false),
      last_record_offset_(0),
      end_of_buffer_offset_(0),
      initial_offset_(initial_offset),
      resyncing_(initial_offset > 0) {
    __builtin_trap() /* STUB: not implemented */;
}

Reader::~Reader() { delete[] backing_store_; }

bool Reader::SkipToInitialBlock() {
    __builtin_trap() /* STUB: not implemented */;
}

bool Reader::ReadRecord(Slice* record, std::string* scratch) {
    __builtin_trap() /* STUB: not implemented */;
}

uint64_t Reader::LastRecordOffset() {
    __builtin_trap() /* STUB: not implemented */;
}

void Reader::ReportCorruption(uint64_t bytes, const char* reason) {
    __builtin_trap() /* STUB: not implemented */;
}

void Reader::ReportDrop(uint64_t bytes, const Status& reason) {
    __builtin_trap() /* STUB: not implemented */;
}

unsigned int Reader::ReadPhysicalRecord(Slice* result) {
    __builtin_trap() /* STUB: not implemented */;
}

}  // namespace log
}  // namespace leveldb
