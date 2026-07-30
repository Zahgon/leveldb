#include <stdexcept>
#include <cstdlib>
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "db/memtable.h"
#include "db/dbformat.h"
#include "leveldb/comparator.h"
#include "leveldb/env.h"
#include "leveldb/iterator.h"
#include "util/coding.h"

namespace leveldb {

static Slice GetLengthPrefixedSlice(const char* data) {
    __builtin_trap() /* STUB: not implemented */;
}

MemTable::MemTable(const InternalKeyComparator& comparator)
    : comparator_(comparator), refs_(0), table_(comparator_, &arena_) {
    __builtin_trap() /* STUB: not implemented */;
}

MemTable::~MemTable() { assert(refs_ == 0); }

size_t MemTable::ApproximateMemoryUsage() {
    __builtin_trap() /* STUB: not implemented */;
}

int MemTable::KeyComparator::operator()(const char* aptr,
                                        const char* bptr) const {
    __builtin_trap() /* STUB: not implemented */;
}

// Encode a suitable internal key target for "target" and return it.
// Uses *scratch as scratch space, and the returned pointer will point
// into this scratch space.
static const char* EncodeKey(std::string* scratch, const Slice& target) {
    __builtin_trap() /* STUB: not implemented */;
}

class MemTableIterator : public Iterator {
 public:
  explicit MemTableIterator(MemTable::Table* table) : iter_(table) {
    __builtin_trap() /* STUB: not implemented */;
}

  MemTableIterator(const MemTableIterator&) = delete;
  MemTableIterator& operator=(const MemTableIterator&) = delete;

  ~MemTableIterator() override = default;

  bool Valid() const override {
    __builtin_trap() /* STUB: not implemented */;
}
  void Seek(const Slice& k) override {
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
  MemTable::Table::Iterator iter_;
  std::string tmp_;  // For passing to EncodeKey
};

Iterator* MemTable::NewIterator() {
    __builtin_trap() /* STUB: not implemented */;
}

void MemTable::Add(SequenceNumber s, ValueType type, const Slice& key,
                   const Slice& value) {
    __builtin_trap() /* STUB: not implemented */;
}

bool MemTable::Get(const LookupKey& key, std::string* value, Status* s) {
    __builtin_trap() /* STUB: not implemented */;
}

}  // namespace leveldb
