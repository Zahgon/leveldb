#include <stdexcept>
#include <cstdlib>
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef STORAGE_LEVELDB_UTIL_RANDOM_H_
#define STORAGE_LEVELDB_UTIL_RANDOM_H_

#include <cstdint>

namespace leveldb {

// A very simple random number generator.  Not especially good at
// generating truly random bits, but good enough for our needs in this
// package.
class Random {
 private:
  uint32_t seed_;

 public:
  explicit Random(uint32_t s) : seed_(s & 0x7fffffffu) {
    __builtin_trap() /* STUB: not implemented */;
}
  uint32_t Next() {
    __builtin_trap() /* STUB: not implemented */;
}
  // Returns a uniformly distributed value in the range [0..n-1]
  // REQUIRES: n > 0
  uint32_t Uniform(int n) {
    __builtin_trap() /* STUB: not implemented */;
}

  // Randomly returns true ~"1/n" of the time, and false otherwise.
  // REQUIRES: n > 0
  bool OneIn(int n) {
    __builtin_trap() /* STUB: not implemented */;
}

  // Skewed: pick "base" uniformly from range [0,max_log] and then
  // return "base" random bits.  The effect is to pick a number in the
  // range [0,2^max_log-1] with exponential bias towards smaller numbers.
  uint32_t Skewed(int max_log) {
    __builtin_trap() /* STUB: not implemented */;
}
};

}  // namespace leveldb

#endif  // STORAGE_LEVELDB_UTIL_RANDOM_H_
