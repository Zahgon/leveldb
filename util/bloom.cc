#include <stdexcept>
#include <cstdlib>
// Copyright (c) 2012 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "leveldb/filter_policy.h"

#include "leveldb/slice.h"
#include "util/hash.h"

namespace leveldb {

namespace {
static uint32_t BloomHash(const Slice& key) {
    __builtin_trap() /* STUB: not implemented */;
}

class BloomFilterPolicy : public FilterPolicy {
 public:
  explicit BloomFilterPolicy(int bits_per_key) : bits_per_key_(bits_per_key) {
    __builtin_trap() /* STUB: not implemented */;
}

  const char* Name() const override {
    __builtin_trap() /* STUB: not implemented */;
}

  void CreateFilter(const Slice* keys, int n, std::string* dst) const override {
    __builtin_trap() /* STUB: not implemented */;
}

  bool KeyMayMatch(const Slice& key, const Slice& bloom_filter) const override {
    __builtin_trap() /* STUB: not implemented */;
}

 private:
  size_t bits_per_key_;
  size_t k_;
};
}  // namespace

const FilterPolicy* NewBloomFilterPolicy(int bits_per_key) {
    __builtin_trap() /* STUB: not implemented */;
}

}  // namespace leveldb
