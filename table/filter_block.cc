#include <stdexcept>
#include <cstdlib>
// Copyright (c) 2012 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "table/filter_block.h"

#include "leveldb/filter_policy.h"
#include "util/coding.h"

namespace leveldb {

// See doc/table_format.md for an explanation of the filter block format.

// Generate new filter every 2KB of data
static const size_t kFilterBaseLg = 11;
static const size_t kFilterBase = 1 << kFilterBaseLg;

FilterBlockBuilder::FilterBlockBuilder(const FilterPolicy* policy)
    : policy_(policy) {
    __builtin_trap() /* STUB: not implemented */;
}

void FilterBlockBuilder::StartBlock(uint64_t block_offset) {
    __builtin_trap() /* STUB: not implemented */;
}

void FilterBlockBuilder::AddKey(const Slice& key) {
    __builtin_trap() /* STUB: not implemented */;
}

Slice FilterBlockBuilder::Finish() {
    __builtin_trap() /* STUB: not implemented */;
}

void FilterBlockBuilder::GenerateFilter() {
    __builtin_trap() /* STUB: not implemented */;
}

FilterBlockReader::FilterBlockReader(const FilterPolicy* policy,
                                     const Slice& contents)
    : policy_(policy), data_(nullptr), offset_(nullptr), num_(0), base_lg_(0) {
    __builtin_trap() /* STUB: not implemented */;
}

bool FilterBlockReader::KeyMayMatch(uint64_t block_offset, const Slice& key) {
    __builtin_trap() /* STUB: not implemented */;
}

}  // namespace leveldb
