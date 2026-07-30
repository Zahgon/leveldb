#include <stdexcept>
#include <cstdlib>
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "gtest/gtest.h"
#include "db/memtable.h"
#include "db/write_batch_internal.h"
#include "leveldb/db.h"
#include "leveldb/env.h"
#include "util/logging.h"

namespace leveldb {

static std::string PrintContents(WriteBatch* b) {
  InternalKeyComparator cmp(BytewiseComparator());
  MemTable* mem = new MemTable(cmp);
  mem->Ref();
  std::string state;
  Status s = WriteBatchInternal::InsertInto(b, mem);
  int count = 0;
  Iterator* iter = mem->NewIterator();
  for (iter->SeekToFirst(); iter->Valid(); iter->Next()) {
    ParsedInternalKey ikey;
    EXPECT_TRUE(ParseInternalKey(iter->key(), &ikey));
    switch (ikey.type) {
      case kTypeValue:
        state.append("Put(");
        state.append(ikey.user_key.ToString());
        state.append(", ");
        state.append(iter->value().ToString());
        state.append(")");
        count++;
        break;
      case kTypeDeletion:
        state.append("Delete(");
        state.append(ikey.user_key.ToString());
        state.append(")");
        count++;
        break;
    }
    state.append("@");
    state.append(NumberToString(ikey.sequence));
  }
  delete iter;
  if (!s.ok()) {
    state.append("ParseError()");
  } else if (count != WriteBatchInternal::Count(b)) {
    state.append("CountMismatch()");
  }
  mem->Unref();
  return state;
}

TEST(WriteBatchTest, Empty) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST(WriteBatchTest, Multiple) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST(WriteBatchTest, Corruption) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST(WriteBatchTest, Append) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST(WriteBatchTest, ApproximateSize) {
    __builtin_trap() /* STUB: not implemented */;
}

}  // namespace leveldb
