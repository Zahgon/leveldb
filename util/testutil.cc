#include <stdexcept>
#include <cstdlib>
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "util/testutil.h"

#include <string>

#include "util/random.h"

namespace leveldb {
namespace test {

Slice RandomString(Random* rnd, int len, std::string* dst) {
    __builtin_trap() /* STUB: not implemented */;
}

std::string RandomKey(Random* rnd, int len) {
    __builtin_trap() /* STUB: not implemented */;
}

Slice CompressibleString(Random* rnd, double compressed_fraction, size_t len,
                         std::string* dst) {
    __builtin_trap() /* STUB: not implemented */;
}

}  // namespace test
}  // namespace leveldb
