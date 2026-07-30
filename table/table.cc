#include <stdexcept>
#include <cstdlib>
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "leveldb/table.h"

#include "leveldb/cache.h"
#include "leveldb/comparator.h"
#include "leveldb/env.h"
#include "leveldb/filter_policy.h"
#include "leveldb/options.h"
#include "table/block.h"
#include "table/filter_block.h"
#include "table/format.h"
#include "table/two_level_iterator.h"
#include "util/coding.h"

namespace leveldb {

struct Table::Rep {
  ~Rep() {
    delete filter;
    delete[] filter_data;
    delete index_block;
  }

  Options options;
  Status status;
  RandomAccessFile* file;
  uint64_t cache_id;
  FilterBlockReader* filter;
  const char* filter_data;

  BlockHandle metaindex_handle;  // Handle to metaindex_block: saved from footer
  Block* index_block;
};

Status Table::Open(const Options& options, RandomAccessFile* file,
                   uint64_t size, Table** table) {
    __builtin_trap() /* STUB: not implemented */;
}

void Table::ReadMeta(const Footer& footer) {
    __builtin_trap() /* STUB: not implemented */;
}

void Table::ReadFilter(const Slice& filter_handle_value) {
    __builtin_trap() /* STUB: not implemented */;
}

Table::~Table() { delete rep_; }

static void DeleteBlock(void* arg, void* ignored) {
    __builtin_trap() /* STUB: not implemented */;
}

static void DeleteCachedBlock(const Slice& key, void* value) {
    __builtin_trap() /* STUB: not implemented */;
}

static void ReleaseBlock(void* arg, void* h) {
    __builtin_trap() /* STUB: not implemented */;
}

// Convert an index iterator value (i.e., an encoded BlockHandle)
// into an iterator over the contents of the corresponding block.
Iterator* Table::BlockReader(void* arg, const ReadOptions& options,
                             const Slice& index_value) {
    __builtin_trap() /* STUB: not implemented */;
}

Iterator* Table::NewIterator(const ReadOptions& options) const {
    __builtin_trap() /* STUB: not implemented */;
}

Status Table::InternalGet(const ReadOptions& options, const Slice& k, void* arg,
                          void (*handle_result)(void*, const Slice&,
                                                const Slice&)) {
    __builtin_trap() /* STUB: not implemented */;
}

uint64_t Table::ApproximateOffsetOf(const Slice& key) const {
    __builtin_trap() /* STUB: not implemented */;
}

}  // namespace leveldb
