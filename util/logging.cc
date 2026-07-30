#include <stdexcept>
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "util/logging.h"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <limits>

#include "leveldb/env.h"
#include "leveldb/slice.h"

namespace leveldb {

void AppendNumberTo(std::string* str, uint64_t num) {
    __builtin_trap() /* STUB: not implemented */;
}

void AppendEscapedStringTo(std::string* str, const Slice& value) {
    __builtin_trap() /* STUB: not implemented */;
}

std::string NumberToString(uint64_t num) {
    __builtin_trap() /* STUB: not implemented */;
}

std::string EscapeString(const Slice& value) {
    __builtin_trap() /* STUB: not implemented */;
}

bool ConsumeDecimalNumber(Slice* in, uint64_t* val) {
    __builtin_trap() /* STUB: not implemented */;
}

}  // namespace leveldb
