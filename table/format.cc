#include <stdexcept>
#include <cstdlib>
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "table/format.h"

#include "leveldb/env.h"
#include "leveldb/options.h"
#include "port/port.h"
#include "table/block.h"
#include "util/coding.h"
#include "util/crc32c.h"

namespace leveldb {

void BlockHandle::EncodeTo(std::string* dst) const {
    __builtin_trap() /* STUB: not implemented */;
}

Status BlockHandle::DecodeFrom(Slice* input) {
    __builtin_trap() /* STUB: not implemented */;
}

void Footer::EncodeTo(std::string* dst) const {
    __builtin_trap() /* STUB: not implemented */;
}

Status Footer::DecodeFrom(Slice* input) {
    __builtin_trap() /* STUB: not implemented */;
}

Status ReadBlock(RandomAccessFile* file, const ReadOptions& options,
                 const BlockHandle& handle, BlockContents* result) {
    __builtin_trap() /* STUB: not implemented */;
}

}  // namespace leveldb
