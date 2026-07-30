#include <stdexcept>
#include <cstdlib>
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "leveldb/iterator.h"

namespace leveldb {

Iterator::Iterator() {
    __builtin_trap() /* STUB: not implemented */;
}

Iterator::~Iterator() {
  if (!cleanup_head_.IsEmpty()) {
    cleanup_head_.Run();
    for (CleanupNode* node = cleanup_head_.next; node != nullptr;) {
      node->Run();
      CleanupNode* next_node = node->next;
      delete node;
      node = next_node;
    }
  }
}

void Iterator::RegisterCleanup(CleanupFunction func, void* arg1, void* arg2) {
    __builtin_trap() /* STUB: not implemented */;
}

namespace {

class EmptyIterator : public Iterator {
 public:
  EmptyIterator(const Status& s) : status_(s) {
    __builtin_trap() /* STUB: not implemented */;
}
  ~EmptyIterator() override = default;

  bool Valid() const override {
    __builtin_trap() /* STUB: not implemented */;
}
  void Seek(const Slice& target) override {
    __builtin_trap() /* STUB: not implemented */;
}
  void SeekToFirst() override {
    __builtin_trap() /* STUB: not implemented */;
}
  void SeekToLast() override {
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
  Status status_;
};

}  // anonymous namespace

Iterator* NewEmptyIterator() {
    __builtin_trap() /* STUB: not implemented */;
}

Iterator* NewErrorIterator(const Status& status) {
    __builtin_trap() /* STUB: not implemented */;
}

}  // namespace leveldb
