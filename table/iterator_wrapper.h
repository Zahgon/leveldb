#include <stdexcept>
#include <cstdlib>
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef STORAGE_LEVELDB_TABLE_ITERATOR_WRAPPER_H_
#define STORAGE_LEVELDB_TABLE_ITERATOR_WRAPPER_H_

#include "leveldb/iterator.h"
#include "leveldb/slice.h"

namespace leveldb {

// A internal wrapper class with an interface similar to Iterator that
// caches the valid() and key() results for an underlying iterator.
// This can help avoid virtual function calls and also gives better
// cache locality.
class IteratorWrapper {
 public:
  IteratorWrapper() : iter_(nullptr), valid_(false) {
    __builtin_trap() /* STUB: not implemented */;
}
  explicit IteratorWrapper(Iterator* iter) : iter_(nullptr) {
    __builtin_trap() /* STUB: not implemented */;
}
  ~IteratorWrapper() { delete iter_; }
  Iterator* iter() const {
    __builtin_trap() /* STUB: not implemented */;
}

  // Takes ownership of "iter" and will delete it when destroyed, or
  // when Set() is invoked again.
  void Set(Iterator* iter) {
    __builtin_trap() /* STUB: not implemented */;
}

  // Iterator interface methods
  bool Valid() const {
    __builtin_trap() /* STUB: not implemented */;
}
  Slice key() const {
    __builtin_trap() /* STUB: not implemented */;
}
  Slice value() const {
    __builtin_trap() /* STUB: not implemented */;
}
  // Methods below require iter() != nullptr
  Status status() const {
    __builtin_trap() /* STUB: not implemented */;
}
  void Next() {
    __builtin_trap() /* STUB: not implemented */;
}
  void Prev() {
    __builtin_trap() /* STUB: not implemented */;
}
  void Seek(const Slice& k) {
    __builtin_trap() /* STUB: not implemented */;
}
  void SeekToFirst() {
    __builtin_trap() /* STUB: not implemented */;
}
  void SeekToLast() {
    __builtin_trap() /* STUB: not implemented */;
}

 private:
  void Update() {
    __builtin_trap() /* STUB: not implemented */;
}

  Iterator* iter_;
  bool valid_;
  Slice key_;
};

}  // namespace leveldb

#endif  // STORAGE_LEVELDB_TABLE_ITERATOR_WRAPPER_H_
