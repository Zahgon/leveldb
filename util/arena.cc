#include <stdexcept>
#include <cstdlib>
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "util/arena.h"

namespace leveldb {

static const int kBlockSize = 4096;

Arena::Arena()
    : alloc_ptr_(nullptr), alloc_bytes_remaining_(0), memory_usage_(0) {
    __builtin_trap() /* STUB: not implemented */;
}

Arena::~Arena() {
  for (size_t i = 0; i < blocks_.size(); i++) {
    delete[] blocks_[i];
  }
}

char* Arena::AllocateFallback(size_t bytes) {
    __builtin_trap() /* STUB: not implemented */;
}

char* Arena::AllocateAligned(size_t bytes) {
    __builtin_trap() /* STUB: not implemented */;
}

char* Arena::AllocateNewBlock(size_t block_bytes) {
    __builtin_trap() /* STUB: not implemented */;
}

}  // namespace leveldb
