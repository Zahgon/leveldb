#include <stdexcept>
#include <cstdlib>
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "util/coding.h"

namespace leveldb {

void PutFixed32(std::string* dst, uint32_t value) {
    __builtin_trap() /* STUB: not implemented */;
}

void PutFixed64(std::string* dst, uint64_t value) {
    __builtin_trap() /* STUB: not implemented */;
}

char* EncodeVarint32(char* dst, uint32_t v) {
    __builtin_trap() /* STUB: not implemented */;
}

void PutVarint32(std::string* dst, uint32_t v) {
    __builtin_trap() /* STUB: not implemented */;
}

char* EncodeVarint64(char* dst, uint64_t v) {
    __builtin_trap() /* STUB: not implemented */;
}

void PutVarint64(std::string* dst, uint64_t v) {
    __builtin_trap() /* STUB: not implemented */;
}

void PutLengthPrefixedSlice(std::string* dst, const Slice& value) {
    __builtin_trap() /* STUB: not implemented */;
}

int VarintLength(uint64_t v) {
    __builtin_trap() /* STUB: not implemented */;
}

const char* GetVarint32PtrFallback(const char* p, const char* limit,
                                   uint32_t* value) {
    __builtin_trap() /* STUB: not implemented */;
}

bool GetVarint32(Slice* input, uint32_t* value) {
    __builtin_trap() /* STUB: not implemented */;
}

const char* GetVarint64Ptr(const char* p, const char* limit, uint64_t* value) {
    __builtin_trap() /* STUB: not implemented */;
}

bool GetVarint64(Slice* input, uint64_t* value) {
    __builtin_trap() /* STUB: not implemented */;
}

bool GetLengthPrefixedSlice(Slice* input, Slice* result) {
    __builtin_trap() /* STUB: not implemented */;
}

}  // namespace leveldb
