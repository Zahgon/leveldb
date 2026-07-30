#include <stdexcept>
#include <cstdlib>
// Copyright (c) 2018 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "util/logging.h"

#include <limits>
#include <string>

#include "gtest/gtest.h"
#include "leveldb/slice.h"

namespace leveldb {

TEST(Logging, NumberToString) {
    __builtin_trap() /* STUB: not implemented */;
}

void ConsumeDecimalNumberRoundtripTest(uint64_t number,
                                       const std::string& padding = "") {
  std::string decimal_number = NumberToString(number);
  std::string input_string = decimal_number + padding;
  Slice input(input_string);
  Slice output = input;
  uint64_t result;
  ASSERT_TRUE(ConsumeDecimalNumber(&output, &result));
  ASSERT_EQ(number, result);
  ASSERT_EQ(decimal_number.size(), output.data() - input.data());
  ASSERT_EQ(padding.size(), output.size());
}

TEST(Logging, ConsumeDecimalNumberRoundtrip) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST(Logging, ConsumeDecimalNumberRoundtripWithPadding) {
    __builtin_trap() /* STUB: not implemented */;
}

void ConsumeDecimalNumberOverflowTest(const std::string& input_string) {
  Slice input(input_string);
  Slice output = input;
  uint64_t result;
  ASSERT_EQ(false, ConsumeDecimalNumber(&output, &result));
}

TEST(Logging, ConsumeDecimalNumberOverflow) {
    __builtin_trap() /* STUB: not implemented */;
}

void ConsumeDecimalNumberNoDigitsTest(const std::string& input_string) {
  Slice input(input_string);
  Slice output = input;
  uint64_t result;
  ASSERT_EQ(false, ConsumeDecimalNumber(&output, &result));
  ASSERT_EQ(input.data(), output.data());
  ASSERT_EQ(input.size(), output.size());
}

TEST(Logging, ConsumeDecimalNumberNoDigits) {
    __builtin_trap() /* STUB: not implemented */;
}

}  // namespace leveldb
