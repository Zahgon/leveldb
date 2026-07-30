#include <stdexcept>
#include <cstdlib>
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "leveldb/table_builder.h"

#include <cassert>

#include "leveldb/comparator.h"
#include "leveldb/env.h"
#include "leveldb/filter_policy.h"
#include "leveldb/options.h"
#include "table/block_builder.h"
#include "table/filter_block.h"
#include "table/format.h"
#include "util/coding.h"
#include "util/crc32c.h"

namespace leveldb {

struct TableBuilder::Rep {
  Rep(const Options& opt, WritableFile* f)
      : options(opt),
        index_block_options(opt),
        file(f),
        offset(0),
        data_block(&options),
        index_block(&index_block_options),
        num_entries(0),
        closed(false),
        filter_block(opt.filter_policy == nullptr
                         ? nullptr
                         : new FilterBlockBuilder(opt.filter_policy)),
        pending_index_entry(false) {
    __builtin_trap() /* STUB: not implemented */;
}

  Options options;
  Options index_block_options;
  WritableFile* file;
  uint64_t offset;
  Status status;
  BlockBuilder data_block;
  BlockBuilder index_block;
  std::string last_key;
  int64_t num_entries;
  bool closed;  // Either Finish() or Abandon() has been called.
  FilterBlockBuilder* filter_block;

  // We do not emit the index entry for a block until we have seen the
  // first key for the next data block.  This allows us to use shorter
  // keys in the index block.  For example, consider a block boundary
  // between the keys "the quick brown fox" and "the who".  We can use
  // "the r" as the key for the index block entry since it is >= all
  // entries in the first block and < all entries in subsequent
  // blocks.
  //
  // Invariant: r->pending_index_entry is true only if data_block is empty.
  bool pending_index_entry;
  BlockHandle pending_handle;  // Handle to add to index block

  std::string compressed_output;
};

TableBuilder::TableBuilder(const Options& options, WritableFile* file)
    : rep_(new Rep(options, file)) {
    __builtin_trap() /* STUB: not implemented */;
}

TableBuilder::~TableBuilder() {
  assert(rep_->closed);  // Catch errors where caller forgot to call Finish()
  delete rep_->filter_block;
  delete rep_;
}

Status TableBuilder::ChangeOptions(const Options& options) {
    __builtin_trap() /* STUB: not implemented */;
}

void TableBuilder::Add(const Slice& key, const Slice& value) {
    __builtin_trap() /* STUB: not implemented */;
}

void TableBuilder::Flush() {
    __builtin_trap() /* STUB: not implemented */;
}

void TableBuilder::WriteBlock(BlockBuilder* block, BlockHandle* handle) {
    __builtin_trap() /* STUB: not implemented */;
}

void TableBuilder::WriteRawBlock(const Slice& block_contents,
                                 CompressionType type, BlockHandle* handle) {
    __builtin_trap() /* STUB: not implemented */;
}

Status TableBuilder::status() const {
    __builtin_trap() /* STUB: not implemented */;
}

Status TableBuilder::Finish() {
    __builtin_trap() /* STUB: not implemented */;
}

void TableBuilder::Abandon() {
    __builtin_trap() /* STUB: not implemented */;
}

uint64_t TableBuilder::NumEntries() const {
    __builtin_trap() /* STUB: not implemented */;
}

uint64_t TableBuilder::FileSize() const {
    __builtin_trap() /* STUB: not implemented */;
}

}  // namespace leveldb
