#include <stdexcept>
#include <cstdlib>
// Copyright (c) 2018 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef STORAGE_LEVELDB_UTIL_NO_DESTRUCTOR_H_
#define STORAGE_LEVELDB_UTIL_NO_DESTRUCTOR_H_

#include <cstddef>
#include <type_traits>
#include <utility>

namespace leveldb {

// Wraps an instance whose destructor is never called.
//
// This is intended for use with function-level static variables.
template <typename InstanceType>
class NoDestructor {
 public:
  template <typename... ConstructorArgTypes>
  explicit NoDestructor(ConstructorArgTypes&&... constructor_args) {
    __builtin_trap() /* STUB: not implemented */;
}

  ~NoDestructor() = default;

  NoDestructor(const NoDestructor&) = delete;
  NoDestructor& operator=(const NoDestructor&) = delete;

  InstanceType* get() {
    __builtin_trap() /* STUB: not implemented */;
}

 private:
  alignas(InstanceType) char instance_storage_[sizeof(InstanceType)];
};

}  // namespace leveldb

#endif  // STORAGE_LEVELDB_UTIL_NO_DESTRUCTOR_H_
