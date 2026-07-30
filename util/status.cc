#include <stdexcept>
#include <cstdlib>
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "leveldb/status.h"

#include <cstdio>

#include "port/port.h"

namespace leveldb {

const char* Status::CopyState(const char* state) {
    __builtin_trap() /* STUB: not implemented */;
}

Status::Status(Code code, const Slice& msg, const Slice& msg2) {
    __builtin_trap() /* STUB: not implemented */;
}

std::string Status::ToString() const {
    __builtin_trap() /* STUB: not implemented */;
}

}  // namespace leveldb
