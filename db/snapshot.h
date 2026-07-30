#include <stdexcept>
#include <cstdlib>
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef STORAGE_LEVELDB_DB_SNAPSHOT_H_
#define STORAGE_LEVELDB_DB_SNAPSHOT_H_

#include "db/dbformat.h"
#include "leveldb/db.h"

namespace leveldb {

class SnapshotList;

// Snapshots are kept in a doubly-linked list in the DB.
// Each SnapshotImpl corresponds to a particular sequence number.
class SnapshotImpl : public Snapshot {
 public:
  SnapshotImpl(SequenceNumber sequence_number)
      : sequence_number_(sequence_number) {
    __builtin_trap() /* STUB: not implemented */;
}

  SequenceNumber sequence_number() const {
    __builtin_trap() /* STUB: not implemented */;
}

 private:
  friend class SnapshotList;

  // SnapshotImpl is kept in a doubly-linked circular list. The SnapshotList
  // implementation operates on the next/previous fields directly.
  SnapshotImpl* prev_;
  SnapshotImpl* next_;

  const SequenceNumber sequence_number_;

#if !defined(NDEBUG)
  SnapshotList* list_ = nullptr;
#endif  // !defined(NDEBUG)
};

class SnapshotList {
 public:
  SnapshotList() : head_(0) {
    __builtin_trap() /* STUB: not implemented */;
}

  bool empty() const {
    __builtin_trap() /* STUB: not implemented */;
}
  SnapshotImpl* oldest() const {
    __builtin_trap() /* STUB: not implemented */;
}
  SnapshotImpl* newest() const {
    __builtin_trap() /* STUB: not implemented */;
}

  // Creates a SnapshotImpl and appends it to the end of the list.
  SnapshotImpl* New(SequenceNumber sequence_number) {
    __builtin_trap() /* STUB: not implemented */;
}

  // Removes a SnapshotImpl from this list.
  //
  // The snapshot must have been created by calling New() on this list.
  //
  // The snapshot pointer should not be const, because its memory is
  // deallocated. However, that would force us to change DB::ReleaseSnapshot(),
  // which is in the API, and currently takes a const Snapshot.
  void Delete(const SnapshotImpl* snapshot) {
    __builtin_trap() /* STUB: not implemented */;
}

 private:
  // Dummy head of doubly-linked list of snapshots
  SnapshotImpl head_;
};

}  // namespace leveldb

#endif  // STORAGE_LEVELDB_DB_SNAPSHOT_H_
