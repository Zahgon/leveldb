#include <stdexcept>
#include <cstdlib>
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "db/version_edit.h"

#include "db/version_set.h"
#include "util/coding.h"

namespace leveldb {

// Tag numbers for serialized VersionEdit.  These numbers are written to
// disk and should not be changed.
enum Tag {
  kComparator = 1,
  kLogNumber = 2,
  kNextFileNumber = 3,
  kLastSequence = 4,
  kCompactPointer = 5,
  kDeletedFile = 6,
  kNewFile = 7,
  // 8 was used for large value refs
  kPrevLogNumber = 9
};

void VersionEdit::Clear() {
    __builtin_trap() /* STUB: not implemented */;
}

void VersionEdit::EncodeTo(std::string* dst) const {
    __builtin_trap() /* STUB: not implemented */;
}

static bool GetInternalKey(Slice* input, InternalKey* dst) {
    __builtin_trap() /* STUB: not implemented */;
}

static bool GetLevel(Slice* input, int* level) {
    __builtin_trap() /* STUB: not implemented */;
}

Status VersionEdit::DecodeFrom(const Slice& src) {
    __builtin_trap() /* STUB: not implemented */;
}

std::string VersionEdit::DebugString() const {
    __builtin_trap() /* STUB: not implemented */;
}

}  // namespace leveldb
