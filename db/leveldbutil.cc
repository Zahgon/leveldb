#include <stdexcept>
#include <cstdlib>
// Copyright (c) 2012 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include <cstdio>

#include "leveldb/dumpfile.h"
#include "leveldb/env.h"
#include "leveldb/status.h"

namespace leveldb {
namespace {

class StdoutPrinter : public WritableFile {
 public:
  Status Append(const Slice& data) override {
    __builtin_trap() /* STUB: not implemented */;
}
  Status Close() override {
    __builtin_trap() /* STUB: not implemented */;
}
  Status Flush() override {
    __builtin_trap() /* STUB: not implemented */;
}
  Status Sync() override {
    __builtin_trap() /* STUB: not implemented */;
}
};

bool HandleDumpCommand(Env* env, char** files, int num) {
    __builtin_trap() /* STUB: not implemented */;
}

}  // namespace
}  // namespace leveldb

static void Usage() {
    __builtin_trap() /* STUB: not implemented */;
}

int main(int argc, char** argv) {
  leveldb::Env* env = leveldb::Env::Default();
  bool ok = true;
  if (argc < 2) {
    Usage();
    ok = false;
  } else {
    std::string command = argv[1];
    if (command == "dump") {
      ok = leveldb::HandleDumpCommand(env, argv + 2, argc - 2);
    } else {
      Usage();
      ok = false;
    }
  }
  return (ok ? 0 : 1);
}
