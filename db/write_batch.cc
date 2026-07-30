#include <stdexcept>
#include <cstdlib>
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// WriteBatch::rep_ :=
//    sequence: fixed64
//    count: fixed32
//    data: record[count]
// record :=
//    kTypeValue varstring varstring         |
//    kTypeDeletion varstring
// varstring :=
//    len: varint32
//    data: uint8[len]

#include "leveldb/write_batch.h"

#include "db/dbformat.h"
#include "db/memtable.h"
#include "db/write_batch_internal.h"
#include "leveldb/db.h"
#include "util/coding.h"

namespace leveldb {

// WriteBatch header has an 8-byte sequence number followed by a 4-byte count.
static const size_t kHeader = 12;

WriteBatch::WriteBatch() {
    __builtin_trap() /* STUB: not implemented */;
}

WriteBatch::~WriteBatch() = default;

WriteBatch::Handler::~Handler() = default;

void WriteBatch::Clear() {
    __builtin_trap() /* STUB: not implemented */;
}

size_t WriteBatch::ApproximateSize() const {
    __builtin_trap() /* STUB: not implemented */;
}

Status WriteBatch::Iterate(Handler* handler) const {
    __builtin_trap() /* STUB: not implemented */;
}

int WriteBatchInternal::Count(const WriteBatch* b) {
    __builtin_trap() /* STUB: not implemented */;
}

void WriteBatchInternal::SetCount(WriteBatch* b, int n) {
    __builtin_trap() /* STUB: not implemented */;
}

SequenceNumber WriteBatchInternal::Sequence(const WriteBatch* b) {
    __builtin_trap() /* STUB: not implemented */;
}

void WriteBatchInternal::SetSequence(WriteBatch* b, SequenceNumber seq) {
    __builtin_trap() /* STUB: not implemented */;
}

void WriteBatch::Put(const Slice& key, const Slice& value) {
    __builtin_trap() /* STUB: not implemented */;
}

void WriteBatch::Delete(const Slice& key) {
    __builtin_trap() /* STUB: not implemented */;
}

void WriteBatch::Append(const WriteBatch& source) {
    __builtin_trap() /* STUB: not implemented */;
}

namespace {
class MemTableInserter : public WriteBatch::Handler {
 public:
  SequenceNumber sequence_;
  MemTable* mem_;

  void Put(const Slice& key, const Slice& value) override {
    __builtin_trap() /* STUB: not implemented */;
}
  void Delete(const Slice& key) override {
    __builtin_trap() /* STUB: not implemented */;
}
};
}  // namespace

Status WriteBatchInternal::InsertInto(const WriteBatch* b, MemTable* memtable) {
    __builtin_trap() /* STUB: not implemented */;
}

void WriteBatchInternal::SetContents(WriteBatch* b, const Slice& contents) {
    __builtin_trap() /* STUB: not implemented */;
}

void WriteBatchInternal::Append(WriteBatch* dst, const WriteBatch* src) {
    __builtin_trap() /* STUB: not implemented */;
}

}  // namespace leveldb
