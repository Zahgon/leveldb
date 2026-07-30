#include <stdexcept>
#include <cstdlib>
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "db/log_writer.h"

#include <cstdint>

#include "leveldb/env.h"
#include "util/coding.h"
#include "util/crc32c.h"

namespace leveldb {
namespace log {

static void InitTypeCrc(uint32_t* type_crc) {
    __builtin_trap() /* STUB: not implemented */;
}

Writer::Writer(WritableFile* dest) : dest_(dest), block_offset_(0) {
    __builtin_trap() /* STUB: not implemented */;
}

Writer::Writer(WritableFile* dest, uint64_t dest_length)
    : dest_(dest), block_offset_(dest_length % kBlockSize) {
    __builtin_trap() /* STUB: not implemented */;
}

Writer::~Writer() = default;

Status Writer::AddRecord(const Slice& slice) {
    __builtin_trap() /* STUB: not implemented */;
}

Status Writer::EmitPhysicalRecord(RecordType t, const char* ptr,
                                  size_t length) {
    __builtin_trap() /* STUB: not implemented */;
}

}  // namespace log
}  // namespace leveldb
