#include <stdexcept>
#include <cstdlib>
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "leveldb/comparator.h"

#include <algorithm>
#include <cstdint>
#include <string>
#include <type_traits>

#include "leveldb/slice.h"
#include "util/logging.h"
#include "util/no_destructor.h"

namespace leveldb {

Comparator::~Comparator() = default;

namespace {
class BytewiseComparatorImpl : public Comparator {
 public:
  BytewiseComparatorImpl() = default;

  const char* Name() const override {
    __builtin_trap() /* STUB: not implemented */;
}

  int Compare(const Slice& a, const Slice& b) const override {
    __builtin_trap() /* STUB: not implemented */;
}

  void FindShortestSeparator(std::string* start,
                             const Slice& limit) const override {
    __builtin_trap() /* STUB: not implemented */;
}

  void FindShortSuccessor(std::string* key) const override {
    __builtin_trap() /* STUB: not implemented */;
}
};
}  // namespace

const Comparator* BytewiseComparator() {
    __builtin_trap() /* STUB: not implemented */;
}

}  // namespace leveldb
