#include <stdexcept>
#include <cstdlib>
// Copyright (c) 2012 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "table/filter_block.h"

#include "gtest/gtest.h"
#include "leveldb/filter_policy.h"
#include "util/coding.h"
#include "util/hash.h"
#include "util/logging.h"
#include "util/testutil.h"

namespace leveldb {

// For testing: emit an array with one hash value per key
class TestHashFilter : public FilterPolicy {
 public:
  const char* Name() const override { return "TestHashFilter"; }

  void CreateFilter(const Slice* keys, int n, std::string* dst) const override {
    for (int i = 0; i < n; i++) {
      uint32_t h = Hash(keys[i].data(), keys[i].size(), 1);
      PutFixed32(dst, h);
    }
  }

  bool KeyMayMatch(const Slice& key, const Slice& filter) const override {
    uint32_t h = Hash(key.data(), key.size(), 1);
    for (size_t i = 0; i + 4 <= filter.size(); i += 4) {
      if (h == DecodeFixed32(filter.data() + i)) {
        return true;
      }
    }
    return false;
  }
};

class FilterBlockTest : public testing::Test {
 public:
  TestHashFilter policy_;
};

TEST_F(FilterBlockTest, EmptyBuilder) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(FilterBlockTest, SingleChunk) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(FilterBlockTest, MultiChunk) {
    __builtin_trap() /* STUB: not implemented */;
}

}  // namespace leveldb
