#include <stdexcept>
#include <cstdlib>
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "helpers/memenv/memenv.h"

#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "db/db_impl.h"
#include "leveldb/db.h"
#include "leveldb/env.h"
#include "util/testutil.h"

namespace leveldb {

class MemEnvTest : public testing::Test {
 public:
  MemEnvTest() : env_(NewMemEnv(Env::Default())) {}
  ~MemEnvTest() { delete env_; }

  Env* env_;
};

TEST_F(MemEnvTest, Basics) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(MemEnvTest, ReadWrite) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(MemEnvTest, Locks) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(MemEnvTest, Misc) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(MemEnvTest, LargeWrite) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(MemEnvTest, OverwriteOpenFile) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(MemEnvTest, DBTest) {
    __builtin_trap() /* STUB: not implemented */;
}

}  // namespace leveldb
