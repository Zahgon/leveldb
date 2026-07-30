#include <stdexcept>
#include <cstdlib>
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "db/version_set.h"

#include "gtest/gtest.h"
#include "util/logging.h"
#include "util/testutil.h"

namespace leveldb {

class FindFileTest : public testing::Test {
 public:
  FindFileTest() : disjoint_sorted_files_(true) {}

  ~FindFileTest() {
    for (int i = 0; i < files_.size(); i++) {
      delete files_[i];
    }
  }

  void Add(const char* smallest, const char* largest,
           SequenceNumber smallest_seq = 100,
           SequenceNumber largest_seq = 100) {
    FileMetaData* f = new FileMetaData;
    f->number = files_.size() + 1;
    f->smallest = InternalKey(smallest, smallest_seq, kTypeValue);
    f->largest = InternalKey(largest, largest_seq, kTypeValue);
    files_.push_back(f);
  }

  int Find(const char* key) {
    InternalKey target(key, 100, kTypeValue);
    InternalKeyComparator cmp(BytewiseComparator());
    return FindFile(cmp, files_, target.Encode());
  }

  bool Overlaps(const char* smallest, const char* largest) {
    InternalKeyComparator cmp(BytewiseComparator());
    Slice s(smallest != nullptr ? smallest : "");
    Slice l(largest != nullptr ? largest : "");
    return SomeFileOverlapsRange(cmp, disjoint_sorted_files_, files_,
                                 (smallest != nullptr ? &s : nullptr),
                                 (largest != nullptr ? &l : nullptr));
  }

  bool disjoint_sorted_files_;

 private:
  std::vector<FileMetaData*> files_;
};

TEST_F(FindFileTest, Empty) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(FindFileTest, Single) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(FindFileTest, Multiple) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(FindFileTest, MultipleNullBoundaries) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(FindFileTest, OverlapSequenceChecks) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(FindFileTest, OverlappingFiles) {
    __builtin_trap() /* STUB: not implemented */;
}

void AddBoundaryInputs(const InternalKeyComparator& icmp,
                       const std::vector<FileMetaData*>& level_files,
                       std::vector<FileMetaData*>* compaction_files);

class AddBoundaryInputsTest : public testing::Test {
 public:
  std::vector<FileMetaData*> level_files_;
  std::vector<FileMetaData*> compaction_files_;
  std::vector<FileMetaData*> all_files_;
  InternalKeyComparator icmp_;

  AddBoundaryInputsTest() : icmp_(BytewiseComparator()) {}

  ~AddBoundaryInputsTest() {
    for (size_t i = 0; i < all_files_.size(); ++i) {
      delete all_files_[i];
    }
    all_files_.clear();
  }

  FileMetaData* CreateFileMetaData(uint64_t number, InternalKey smallest,
                                   InternalKey largest) {
    FileMetaData* f = new FileMetaData();
    f->number = number;
    f->smallest = smallest;
    f->largest = largest;
    all_files_.push_back(f);
    return f;
  }
};

TEST_F(AddBoundaryInputsTest, TestEmptyFileSets) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(AddBoundaryInputsTest, TestEmptyLevelFiles) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(AddBoundaryInputsTest, TestEmptyCompactionFiles) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(AddBoundaryInputsTest, TestNoBoundaryFiles) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(AddBoundaryInputsTest, TestOneBoundaryFiles) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(AddBoundaryInputsTest, TestTwoBoundaryFiles) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(AddBoundaryInputsTest, TestDisjoinFilePointers) {
    __builtin_trap() /* STUB: not implemented */;
}

}  // namespace leveldb
