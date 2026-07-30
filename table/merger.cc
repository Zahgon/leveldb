#include <stdexcept>
#include <cstdlib>
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "table/merger.h"

#include "leveldb/comparator.h"
#include "leveldb/iterator.h"
#include "table/iterator_wrapper.h"

namespace leveldb {

namespace {
class MergingIterator : public Iterator {
 public:
  MergingIterator(const Comparator* comparator, Iterator** children, int n)
      : comparator_(comparator),
        children_(new IteratorWrapper[n]),
        n_(n),
        current_(nullptr),
        direction_(kForward) {
    __builtin_trap() /* STUB: not implemented */;
}

  ~MergingIterator() override { delete[] children_; }

  bool Valid() const override {
    __builtin_trap() /* STUB: not implemented */;
}

  void SeekToFirst() override {
    __builtin_trap() /* STUB: not implemented */;
}

  void SeekToLast() override {
    __builtin_trap() /* STUB: not implemented */;
}

  void Seek(const Slice& target) override {
    __builtin_trap() /* STUB: not implemented */;
}

  void Next() override {
    __builtin_trap() /* STUB: not implemented */;
}

  void Prev() override {
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
  // Which direction is the iterator moving?
  enum Direction { kForward, kReverse };

  void FindSmallest();
  void FindLargest();

  // We might want to use a heap in case there are lots of children.
  // For now we use a simple array since we expect a very small number
  // of children in leveldb.
  const Comparator* comparator_;
  IteratorWrapper* children_;
  int n_;
  IteratorWrapper* current_;
  Direction direction_;
};

void MergingIterator::FindSmallest() {
    __builtin_trap() /* STUB: not implemented */;
}

void MergingIterator::FindLargest() {
    __builtin_trap() /* STUB: not implemented */;
}
}  // namespace

Iterator* NewMergingIterator(const Comparator* comparator, Iterator** children,
                             int n) {
    __builtin_trap() /* STUB: not implemented */;
}

}  // namespace leveldb
