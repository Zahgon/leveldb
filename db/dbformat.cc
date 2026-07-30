#include <stdexcept>
#include <cstdlib>
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "db/dbformat.h"

#include <cstdio>
#include <sstream>

#include "port/port.h"
#include "util/coding.h"

namespace leveldb {

static uint64_t PackSequenceAndType(uint64_t seq, ValueType t) {
    __builtin_trap() /* STUB: not implemented */;
}

void AppendInternalKey(std::string* result, const ParsedInternalKey& key) {
    __builtin_trap() /* STUB: not implemented */;
}

std::string ParsedInternalKey::DebugString() const {
    __builtin_trap() /* STUB: not implemented */;
}

std::string InternalKey::DebugString() const {
    __builtin_trap() /* STUB: not implemented */;
}

const char* InternalKeyComparator::Name() const {
    __builtin_trap() /* STUB: not implemented */;
}

int InternalKeyComparator::Compare(const Slice& akey, const Slice& bkey) const {
    __builtin_trap() /* STUB: not implemented */;
}

void InternalKeyComparator::FindShortestSeparator(std::string* start,
                                                  const Slice& limit) const {
    __builtin_trap() /* STUB: not implemented */;
}

void InternalKeyComparator::FindShortSuccessor(std::string* key) const {
    __builtin_trap() /* STUB: not implemented */;
}

const char* InternalFilterPolicy::Name() const {
    __builtin_trap() /* STUB: not implemented */;
}

void InternalFilterPolicy::CreateFilter(const Slice* keys, int n,
                                        std::string* dst) const {
    __builtin_trap() /* STUB: not implemented */;
}

bool InternalFilterPolicy::KeyMayMatch(const Slice& key, const Slice& f) const {
    __builtin_trap() /* STUB: not implemented */;
}

LookupKey::LookupKey(const Slice& user_key, SequenceNumber s) {
    __builtin_trap() /* STUB: not implemented */;
}

}  // namespace leveldb
