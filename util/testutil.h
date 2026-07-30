#include <stdexcept>
#include <cstdlib>
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef STORAGE_LEVELDB_UTIL_TESTUTIL_H_
#define STORAGE_LEVELDB_UTIL_TESTUTIL_H_

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "helpers/memenv/memenv.h"
#include "leveldb/env.h"
#include "leveldb/slice.h"
#include "util/random.h"

namespace leveldb {
namespace test {

MATCHER(IsOK, "") {
    __builtin_trap() /* STUB: not implemented */;
}

// Macros for testing the results of functions that return leveldb::Status or
// absl::StatusOr<T> (for any type T).
#define EXPECT_LEVELDB_OK(expression) \
  EXPECT_THAT(expression, leveldb::test::IsOK())
#define ASSERT_LEVELDB_OK(expression) \
  ASSERT_THAT(expression, leveldb::test::IsOK())

// Returns the random seed used at the start of the current test run.
inline int RandomSeed() {
    __builtin_trap() /* STUB: not implemented */;
}

// Store in *dst a random string of length "len" and return a Slice that
// references the generated data.
Slice RandomString(Random* rnd, int len, std::string* dst);

// Return a random key with the specified length that may contain interesting
// characters (e.g. \x00, \xff, etc.).
std::string RandomKey(Random* rnd, int len);

// Store in *dst a string of length "len" that will compress to
// "N*compressed_fraction" bytes and return a Slice that references
// the generated data.
Slice CompressibleString(Random* rnd, double compressed_fraction, size_t len,
                         std::string* dst);

// A wrapper that allows injection of errors.
class ErrorEnv : public EnvWrapper {
 public:
  bool writable_file_error_;
  int num_writable_file_errors_;

  ErrorEnv()
      : EnvWrapper(NewMemEnv(Env::Default())),
        writable_file_error_(false),
        num_writable_file_errors_(0) {
    __builtin_trap() /* STUB: not implemented */;
}
  ~ErrorEnv() override { delete target(); }

  Status NewWritableFile(const std::string& fname,
                         WritableFile** result) override {
    __builtin_trap() /* STUB: not implemented */;
}

  Status NewAppendableFile(const std::string& fname,
                           WritableFile** result) override {
    __builtin_trap() /* STUB: not implemented */;
}
};

}  // namespace test
}  // namespace leveldb

#endif  // STORAGE_LEVELDB_UTIL_TESTUTIL_H_
