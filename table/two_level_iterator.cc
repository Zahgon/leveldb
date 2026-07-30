#include <stdexcept>
#include <cstdlib>
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "table/two_level_iterator.h"

#include "leveldb/table.h"
#include "table/block.h"
#include "table/format.h"
#include "table/iterator_wrapper.h"

namespace leveldb {

namespace {

typedef Iterator* (*BlockFunction)(void*, const ReadOptions&, const Slice&);

class TwoLevelIterator : public Iterator {
 public:
  TwoLevelIterator(Iterator* index_iter, BlockFunction block_function,
                   void* arg, const ReadOptions& options);

  ~TwoLevelIterator() override;

  void Seek(const Slice& target) override;
  void SeekToFirst() override;
  void SeekToLast() override;
  void Next() override;
  void Prev() override;

  bool Valid() const override {
    __builtin_trap() /* STUB: not implemented */;
}
  Slice key() const override {
    __builtin_trap() /* STUB: not implemented */;
}
  Slice value() const override {
    __builtin_trap() /* STUB: not implemented */;
}
  Status status() const override {
    __builtin_trap() /* STUB: not implemented */;
}

 private:
  void SaveError(const Status& s) {
    __builtin_trap() /* STUB: not implemented */;
}
  void SkipEmptyDataBlocksForward();
  void SkipEmptyDataBlocksBackward();
  void SetDataIterator(Iterator* data_iter);
  void InitDataBlock();

  BlockFunction block_function_;
  void* arg_;
  const ReadOptions options_;
  Status status_;
  IteratorWrapper index_iter_;
  IteratorWrapper data_iter_;  // May be nullptr
  // If data_iter_ is non-null, then "data_block_handle_" holds the
  // "index_value" passed to block_function_ to create the data_iter_.
  std::string data_block_handle_;
};

TwoLevelIterator::TwoLevelIterator(Iterator* index_iter,
                                   BlockFunction block_function, void* arg,
                                   const ReadOptions& options)
    : block_function_(block_function),
      arg_(arg),
      options_(options),
      index_iter_(index_iter),
      data_iter_(nullptr) {
    __builtin_trap() /* STUB: not implemented */;
}

TwoLevelIterator::~TwoLevelIterator() = default;

void TwoLevelIterator::Seek(const Slice& target) {
    __builtin_trap() /* STUB: not implemented */;
}

void TwoLevelIterator::SeekToFirst() {
    __builtin_trap() /* STUB: not implemented */;
}

void TwoLevelIterator::SeekToLast() {
    __builtin_trap() /* STUB: not implemented */;
}

void TwoLevelIterator::Next() {
    __builtin_trap() /* STUB: not implemented */;
}

void TwoLevelIterator::Prev() {
    __builtin_trap() /* STUB: not implemented */;
}

void TwoLevelIterator::SkipEmptyDataBlocksForward() {
    __builtin_trap() /* STUB: not implemented */;
}

void TwoLevelIterator::SkipEmptyDataBlocksBackward() {
    __builtin_trap() /* STUB: not implemented */;
}

void TwoLevelIterator::SetDataIterator(Iterator* data_iter) {
    __builtin_trap() /* STUB: not implemented */;
}

void TwoLevelIterator::InitDataBlock() {
    __builtin_trap() /* STUB: not implemented */;
}

}  // namespace

Iterator* NewTwoLevelIterator(Iterator* index_iter,
                              BlockFunction block_function, void* arg,
                              const ReadOptions& options) {
    __builtin_trap() /* STUB: not implemented */;
}

}  // namespace leveldb
