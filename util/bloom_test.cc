#include <stdexcept>
#include <cstdlib>
// Copyright (c) 2012 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "gtest/gtest.h"
#include "leveldb/filter_policy.h"
#include "util/coding.h"
#include "util/logging.h"
#include "util/testutil.h"

namespace leveldb {

static const int kVerbose = 1;

static Slice Key(int i, char* buffer) {
  EncodeFixed32(buffer, i);
  return Slice(buffer, sizeof(uint32_t));
}

class BloomTest : public testing::Test {
 public:
  BloomTest() : policy_(NewBloomFilterPolicy(10)) {}

  ~BloomTest() { delete policy_; }

  void Reset() {
    keys_.clear();
    filter_.clear();
  }

  void Add(const Slice& s) { keys_.push_back(s.ToString()); }

  void Build() {
    std::vector<Slice> key_slices;
    for (size_t i = 0; i < keys_.size(); i++) {
      key_slices.push_back(Slice(keys_[i]));
    }
    filter_.clear();
    policy_->CreateFilter(&key_slices[0], static_cast<int>(key_slices.size()),
                          &filter_);
    keys_.clear();
    if (kVerbose >= 2) DumpFilter();
  }

  size_t FilterSize() const { return filter_.size(); }

  void DumpFilter() {
    std::fprintf(stderr, "F(");
    for (size_t i = 0; i + 1 < filter_.size(); i++) {
      const unsigned int c = static_cast<unsigned int>(filter_[i]);
      for (int j = 0; j < 8; j++) {
        std::fprintf(stderr, "%c", (c & (1 << j)) ? '1' : '.');
      }
    }
    std::fprintf(stderr, ")\n");
  }

  bool Matches(const Slice& s) {
    if (!keys_.empty()) {
      Build();
    }
    return policy_->KeyMayMatch(s, filter_);
  }

  double FalsePositiveRate() {
    char buffer[sizeof(int)];
    int result = 0;
    for (int i = 0; i < 10000; i++) {
      if (Matches(Key(i + 1000000000, buffer))) {
        result++;
      }
    }
    return result / 10000.0;
  }

 private:
  const FilterPolicy* policy_;
  std::string filter_;
  std::vector<std::string> keys_;
};

TEST_F(BloomTest, EmptyFilter) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(BloomTest, Small) {
    __builtin_trap() /* STUB: not implemented */;
}

static int NextLength(int length) {
  if (length < 10) {
    length += 1;
  } else if (length < 100) {
    length += 10;
  } else if (length < 1000) {
    length += 100;
  } else {
    length += 1000;
  }
  return length;
}

TEST_F(BloomTest, VaryingLengths) {
    __builtin_trap() /* STUB: not implemented */;
}

// Different bits-per-byte

}  // namespace leveldb
