#include <stdexcept>
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "leveldb/c.h"

#include <string.h>

#include <cstdint>
#include <cstdlib>

#include "leveldb/cache.h"
#include "leveldb/comparator.h"
#include "leveldb/db.h"
#include "leveldb/env.h"
#include "leveldb/filter_policy.h"
#include "leveldb/iterator.h"
#include "leveldb/options.h"
#include "leveldb/status.h"
#include "leveldb/write_batch.h"

using leveldb::Cache;
using leveldb::Comparator;
using leveldb::CompressionType;
using leveldb::DB;
using leveldb::Env;
using leveldb::FileLock;
using leveldb::FilterPolicy;
using leveldb::Iterator;
using leveldb::kMajorVersion;
using leveldb::kMinorVersion;
using leveldb::Logger;
using leveldb::NewBloomFilterPolicy;
using leveldb::NewLRUCache;
using leveldb::Options;
using leveldb::RandomAccessFile;
using leveldb::Range;
using leveldb::ReadOptions;
using leveldb::SequentialFile;
using leveldb::Slice;
using leveldb::Snapshot;
using leveldb::Status;
using leveldb::WritableFile;
using leveldb::WriteBatch;
using leveldb::WriteOptions;

extern "C" {

struct leveldb_t {
  DB* rep;
};
struct leveldb_iterator_t {
  Iterator* rep;
};
struct leveldb_writebatch_t {
  WriteBatch rep;
};
struct leveldb_snapshot_t {
  const Snapshot* rep;
};
struct leveldb_readoptions_t {
  ReadOptions rep;
};
struct leveldb_writeoptions_t {
  WriteOptions rep;
};
struct leveldb_options_t {
  Options rep;
};
struct leveldb_cache_t {
  Cache* rep;
};
struct leveldb_seqfile_t {
  SequentialFile* rep;
};
struct leveldb_randomfile_t {
  RandomAccessFile* rep;
};
struct leveldb_writablefile_t {
  WritableFile* rep;
};
struct leveldb_logger_t {
  Logger* rep;
};
struct leveldb_filelock_t {
  FileLock* rep;
};

struct leveldb_comparator_t : public Comparator {
  ~leveldb_comparator_t() override { (*destructor_)(state_); }

  int Compare(const Slice& a, const Slice& b) const override {
    __builtin_trap() /* STUB: not implemented */;
}

  const char* Name() const override {
    __builtin_trap() /* STUB: not implemented */;
}

  // No-ops since the C binding does not support key shortening methods.
  void FindShortestSeparator(std::string*, const Slice&) const override {
    __builtin_trap() /* STUB: not implemented */;
}
  void FindShortSuccessor(std::string* key) const override {
    __builtin_trap() /* STUB: not implemented */;
}

  void* state_;
  void (*destructor_)(void*);
  int (*compare_)(void*, const char* a, size_t alen, const char* b,
                  size_t blen);
  const char* (*name_)(void*);
};

struct leveldb_filterpolicy_t : public FilterPolicy {
  ~leveldb_filterpolicy_t() override { (*destructor_)(state_); }

  const char* Name() const override {
    __builtin_trap() /* STUB: not implemented */;
}

  void CreateFilter(const Slice* keys, int n, std::string* dst) const override {
    __builtin_trap() /* STUB: not implemented */;
}

  bool KeyMayMatch(const Slice& key, const Slice& filter) const override {
    __builtin_trap() /* STUB: not implemented */;
}

  void* state_;
  void (*destructor_)(void*);
  const char* (*name_)(void*);
  char* (*create_)(void*, const char* const* key_array,
                   const size_t* key_length_array, int num_keys,
                   size_t* filter_length);
  uint8_t (*key_match_)(void*, const char* key, size_t length,
                        const char* filter, size_t filter_length);
};

struct leveldb_env_t {
  Env* rep;
  bool is_default;
};

static bool SaveError(char** errptr, const Status& s) {
    __builtin_trap() /* STUB: not implemented */;
}

static char* CopyString(const std::string& str) {
    __builtin_trap() /* STUB: not implemented */;
}

leveldb_t* leveldb_open(const leveldb_options_t* options, const char* name,
                        char** errptr) {
    __builtin_trap() /* STUB: not implemented */;
}

void leveldb_close(leveldb_t* db) {
    __builtin_trap() /* STUB: not implemented */;
}

void leveldb_put(leveldb_t* db, const leveldb_writeoptions_t* options,
                 const char* key, size_t keylen, const char* val, size_t vallen,
                 char** errptr) {
    __builtin_trap() /* STUB: not implemented */;
}

void leveldb_delete(leveldb_t* db, const leveldb_writeoptions_t* options,
                    const char* key, size_t keylen, char** errptr) {
    __builtin_trap() /* STUB: not implemented */;
}

void leveldb_write(leveldb_t* db, const leveldb_writeoptions_t* options,
                   leveldb_writebatch_t* batch, char** errptr) {
    __builtin_trap() /* STUB: not implemented */;
}

char* leveldb_get(leveldb_t* db, const leveldb_readoptions_t* options,
                  const char* key, size_t keylen, size_t* vallen,
                  char** errptr) {
    __builtin_trap() /* STUB: not implemented */;
}

leveldb_iterator_t* leveldb_create_iterator(
    leveldb_t* db, const leveldb_readoptions_t* options) {
    __builtin_trap() /* STUB: not implemented */;
}

const leveldb_snapshot_t* leveldb_create_snapshot(leveldb_t* db) {
    __builtin_trap() /* STUB: not implemented */;
}

void leveldb_release_snapshot(leveldb_t* db,
                              const leveldb_snapshot_t* snapshot) {
    __builtin_trap() /* STUB: not implemented */;
}

char* leveldb_property_value(leveldb_t* db, const char* propname) {
    __builtin_trap() /* STUB: not implemented */;
}

void leveldb_approximate_sizes(leveldb_t* db, int num_ranges,
                               const char* const* range_start_key,
                               const size_t* range_start_key_len,
                               const char* const* range_limit_key,
                               const size_t* range_limit_key_len,
                               uint64_t* sizes) {
    __builtin_trap() /* STUB: not implemented */;
}

void leveldb_compact_range(leveldb_t* db, const char* start_key,
                           size_t start_key_len, const char* limit_key,
                           size_t limit_key_len) {
    __builtin_trap() /* STUB: not implemented */;
}

void leveldb_destroy_db(const leveldb_options_t* options, const char* name,
                        char** errptr) {
    __builtin_trap() /* STUB: not implemented */;
}

void leveldb_repair_db(const leveldb_options_t* options, const char* name,
                       char** errptr) {
    __builtin_trap() /* STUB: not implemented */;
}

void leveldb_iter_destroy(leveldb_iterator_t* iter) {
    __builtin_trap() /* STUB: not implemented */;
}

uint8_t leveldb_iter_valid(const leveldb_iterator_t* iter) {
    __builtin_trap() /* STUB: not implemented */;
}

void leveldb_iter_seek_to_first(leveldb_iterator_t* iter) {
    __builtin_trap() /* STUB: not implemented */;
}

void leveldb_iter_seek_to_last(leveldb_iterator_t* iter) {
    __builtin_trap() /* STUB: not implemented */;
}

void leveldb_iter_seek(leveldb_iterator_t* iter, const char* k, size_t klen) {
    __builtin_trap() /* STUB: not implemented */;
}

void leveldb_iter_next(leveldb_iterator_t* iter) {
    __builtin_trap() /* STUB: not implemented */;
}

void leveldb_iter_prev(leveldb_iterator_t* iter) {
    __builtin_trap() /* STUB: not implemented */;
}

const char* leveldb_iter_key(const leveldb_iterator_t* iter, size_t* klen) {
    __builtin_trap() /* STUB: not implemented */;
}

const char* leveldb_iter_value(const leveldb_iterator_t* iter, size_t* vlen) {
    __builtin_trap() /* STUB: not implemented */;
}

void leveldb_iter_get_error(const leveldb_iterator_t* iter, char** errptr) {
    __builtin_trap() /* STUB: not implemented */;
}

leveldb_writebatch_t* leveldb_writebatch_create() {
    __builtin_trap() /* STUB: not implemented */;
}

void leveldb_writebatch_destroy(leveldb_writebatch_t* b) {
    __builtin_trap() /* STUB: not implemented */;
}

void leveldb_writebatch_clear(leveldb_writebatch_t* b) {
    __builtin_trap() /* STUB: not implemented */;
}

void leveldb_writebatch_put(leveldb_writebatch_t* b, const char* key,
                            size_t klen, const char* val, size_t vlen) {
    __builtin_trap() /* STUB: not implemented */;
}

void leveldb_writebatch_delete(leveldb_writebatch_t* b, const char* key,
                               size_t klen) {
    __builtin_trap() /* STUB: not implemented */;
}

void leveldb_writebatch_iterate(const leveldb_writebatch_t* b, void* state,
                                void (*put)(void*, const char* k, size_t klen,
                                            const char* v, size_t vlen),
                                void (*deleted)(void*, const char* k,
                                                size_t klen)) {
    __builtin_trap() /* STUB: not implemented */;
}

void leveldb_writebatch_append(leveldb_writebatch_t* destination,
                               const leveldb_writebatch_t* source) {
    __builtin_trap() /* STUB: not implemented */;
}

leveldb_options_t* leveldb_options_create() {
    __builtin_trap() /* STUB: not implemented */;
}

void leveldb_options_destroy(leveldb_options_t* options) {
    __builtin_trap() /* STUB: not implemented */;
}

void leveldb_options_set_comparator(leveldb_options_t* opt,
                                    leveldb_comparator_t* cmp) {
    __builtin_trap() /* STUB: not implemented */;
}

void leveldb_options_set_filter_policy(leveldb_options_t* opt,
                                       leveldb_filterpolicy_t* policy) {
    __builtin_trap() /* STUB: not implemented */;
}

void leveldb_options_set_create_if_missing(leveldb_options_t* opt, uint8_t v) {
    __builtin_trap() /* STUB: not implemented */;
}

void leveldb_options_set_error_if_exists(leveldb_options_t* opt, uint8_t v) {
    __builtin_trap() /* STUB: not implemented */;
}

void leveldb_options_set_paranoid_checks(leveldb_options_t* opt, uint8_t v) {
    __builtin_trap() /* STUB: not implemented */;
}

void leveldb_options_set_env(leveldb_options_t* opt, leveldb_env_t* env) {
    __builtin_trap() /* STUB: not implemented */;
}

void leveldb_options_set_info_log(leveldb_options_t* opt, leveldb_logger_t* l) {
    __builtin_trap() /* STUB: not implemented */;
}

void leveldb_options_set_write_buffer_size(leveldb_options_t* opt, size_t s) {
    __builtin_trap() /* STUB: not implemented */;
}

void leveldb_options_set_max_open_files(leveldb_options_t* opt, int n) {
    __builtin_trap() /* STUB: not implemented */;
}

void leveldb_options_set_cache(leveldb_options_t* opt, leveldb_cache_t* c) {
    __builtin_trap() /* STUB: not implemented */;
}

void leveldb_options_set_block_size(leveldb_options_t* opt, size_t s) {
    __builtin_trap() /* STUB: not implemented */;
}

void leveldb_options_set_block_restart_interval(leveldb_options_t* opt, int n) {
    __builtin_trap() /* STUB: not implemented */;
}

void leveldb_options_set_max_file_size(leveldb_options_t* opt, size_t s) {
    __builtin_trap() /* STUB: not implemented */;
}

void leveldb_options_set_compression(leveldb_options_t* opt, int t) {
    __builtin_trap() /* STUB: not implemented */;
}

leveldb_comparator_t* leveldb_comparator_create(
    void* state, void (*destructor)(void*),
    int (*compare)(void*, const char* a, size_t alen, const char* b,
                   size_t blen),
    const char* (*name)(void*)) {
    __builtin_trap() /* STUB: not implemented */;
}

void leveldb_comparator_destroy(leveldb_comparator_t* cmp) {
    __builtin_trap() /* STUB: not implemented */;
}

leveldb_filterpolicy_t* leveldb_filterpolicy_create(
    void* state, void (*destructor)(void*),
    char* (*create_filter)(void*, const char* const* key_array,
                           const size_t* key_length_array, int num_keys,
                           size_t* filter_length),
    uint8_t (*key_may_match)(void*, const char* key, size_t length,
                             const char* filter, size_t filter_length),
    const char* (*name)(void*)) {
    __builtin_trap() /* STUB: not implemented */;
}

void leveldb_filterpolicy_destroy(leveldb_filterpolicy_t* filter) {
    __builtin_trap() /* STUB: not implemented */;
}

leveldb_filterpolicy_t* leveldb_filterpolicy_create_bloom(int bits_per_key) {
    __builtin_trap() /* STUB: not implemented */;
}

leveldb_readoptions_t* leveldb_readoptions_create() {
    __builtin_trap() /* STUB: not implemented */;
}

void leveldb_readoptions_destroy(leveldb_readoptions_t* opt) {
    __builtin_trap() /* STUB: not implemented */;
}

void leveldb_readoptions_set_verify_checksums(leveldb_readoptions_t* opt,
                                              uint8_t v) {
    __builtin_trap() /* STUB: not implemented */;
}

void leveldb_readoptions_set_fill_cache(leveldb_readoptions_t* opt, uint8_t v) {
    __builtin_trap() /* STUB: not implemented */;
}

void leveldb_readoptions_set_snapshot(leveldb_readoptions_t* opt,
                                      const leveldb_snapshot_t* snap) {
    __builtin_trap() /* STUB: not implemented */;
}

leveldb_writeoptions_t* leveldb_writeoptions_create() {
    __builtin_trap() /* STUB: not implemented */;
}

void leveldb_writeoptions_destroy(leveldb_writeoptions_t* opt) {
    __builtin_trap() /* STUB: not implemented */;
}

void leveldb_writeoptions_set_sync(leveldb_writeoptions_t* opt, uint8_t v) {
    __builtin_trap() /* STUB: not implemented */;
}

leveldb_cache_t* leveldb_cache_create_lru(size_t capacity) {
    __builtin_trap() /* STUB: not implemented */;
}

void leveldb_cache_destroy(leveldb_cache_t* cache) {
    __builtin_trap() /* STUB: not implemented */;
}

leveldb_env_t* leveldb_create_default_env() {
    __builtin_trap() /* STUB: not implemented */;
}

void leveldb_env_destroy(leveldb_env_t* env) {
    __builtin_trap() /* STUB: not implemented */;
}

char* leveldb_env_get_test_directory(leveldb_env_t* env) {
    __builtin_trap() /* STUB: not implemented */;
}

void leveldb_free(void* ptr) {
    __builtin_trap() /* STUB: not implemented */;
}

int leveldb_major_version() {
    __builtin_trap() /* STUB: not implemented */;
}

int leveldb_minor_version() {
    __builtin_trap() /* STUB: not implemented */;
}

}  // end extern "C"
