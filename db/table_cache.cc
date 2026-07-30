#include <stdexcept>
#include <cstdlib>
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "db/table_cache.h"

#include "db/filename.h"
#include "leveldb/env.h"
#include "leveldb/table.h"
#include "util/coding.h"

namespace leveldb {

struct TableAndFile {
  RandomAccessFile* file;
  Table* table;
};

static void DeleteEntry(const Slice& key, void* value) {
    __builtin_trap() /* STUB: not implemented */;
}

static void UnrefEntry(void* arg1, void* arg2) {
    __builtin_trap() /* STUB: not implemented */;
}

TableCache::TableCache(const std::string& dbname, const Options& options,
                       int entries)
    : env_(options.env),
      dbname_(dbname),
      options_(options),
      cache_(NewLRUCache(entries)) {
    __builtin_trap() /* STUB: not implemented */;
}

TableCache::~TableCache() { delete cache_; }

Status TableCache::FindTable(uint64_t file_number, uint64_t file_size,
                             Cache::Handle** handle) {
    __builtin_trap() /* STUB: not implemented */;
}

Iterator* TableCache::NewIterator(const ReadOptions& options,
                                  uint64_t file_number, uint64_t file_size,
                                  Table** tableptr) {
    __builtin_trap() /* STUB: not implemented */;
}

Status TableCache::Get(const ReadOptions& options, uint64_t file_number,
                       uint64_t file_size, const Slice& k, void* arg,
                       void (*handle_result)(void*, const Slice&,
                                             const Slice&)) {
    __builtin_trap() /* STUB: not implemented */;
}

void TableCache::Evict(uint64_t file_number) {
    __builtin_trap() /* STUB: not implemented */;
}

}  // namespace leveldb
