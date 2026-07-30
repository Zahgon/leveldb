#include <stdexcept>
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "leveldb/cache.h"

#include <cassert>
#include <cstdio>
#include <cstdlib>

#include "port/port.h"
#include "port/thread_annotations.h"
#include "util/hash.h"
#include "util/mutexlock.h"

namespace leveldb {

Cache::~Cache() {}

namespace {

// LRU cache implementation
//
// Cache entries have an "in_cache" boolean indicating whether the cache has a
// reference on the entry.  The only ways that this can become false without the
// entry being passed to its "deleter" are via Erase(), via Insert() when
// an element with a duplicate key is inserted, or on destruction of the cache.
//
// The cache keeps two linked lists of items in the cache.  All items in the
// cache are in one list or the other, and never both.  Items still referenced
// by clients but erased from the cache are in neither list.  The lists are:
// - in-use:  contains the items currently referenced by clients, in no
//   particular order.  (This list is used for invariant checking.  If we
//   removed the check, elements that would otherwise be on this list could be
//   left as disconnected singleton lists.)
// - LRU:  contains the items not currently referenced by clients, in LRU order
// Elements are moved between these lists by the Ref() and Unref() methods,
// when they detect an element in the cache acquiring or losing its only
// external reference.

// An entry is a variable length heap-allocated structure.  Entries
// are kept in a circular doubly linked list ordered by access time.
struct LRUHandle {
  void* value;
  void (*deleter)(const Slice&, void* value);
  LRUHandle* next_hash;
  LRUHandle* next;
  LRUHandle* prev;
  size_t charge;  // TODO(opt): Only allow uint32_t?
  size_t key_length;
  bool in_cache;     // Whether entry is in the cache.
  uint32_t refs;     // References, including cache reference, if present.
  uint32_t hash;     // Hash of key(); used for fast sharding and comparisons
  char key_data[1];  // Beginning of key

  Slice key() const {
    __builtin_trap() /* STUB: not implemented */;
}
};

// We provide our own simple hash table since it removes a whole bunch
// of porting hacks and is also faster than some of the built-in hash
// table implementations in some of the compiler/runtime combinations
// we have tested.  E.g., readrandom speeds up by ~5% over the g++
// 4.4.3's builtin hashtable.
class HandleTable {
 public:
  HandleTable() : length_(0), elems_(0), list_(nullptr) {
    __builtin_trap() /* STUB: not implemented */;
}
  ~HandleTable() { delete[] list_; }

  LRUHandle* Lookup(const Slice& key, uint32_t hash) {
    __builtin_trap() /* STUB: not implemented */;
}

  LRUHandle* Insert(LRUHandle* h) {
    __builtin_trap() /* STUB: not implemented */;
}

  LRUHandle* Remove(const Slice& key, uint32_t hash) {
    __builtin_trap() /* STUB: not implemented */;
}

 private:
  // The table consists of an array of buckets where each bucket is
  // a linked list of cache entries that hash into the bucket.
  uint32_t length_;
  uint32_t elems_;
  LRUHandle** list_;

  // Return a pointer to slot that points to a cache entry that
  // matches key/hash.  If there is no such cache entry, return a
  // pointer to the trailing slot in the corresponding linked list.
  LRUHandle** FindPointer(const Slice& key, uint32_t hash) {
    __builtin_trap() /* STUB: not implemented */;
}

  void Resize() {
    __builtin_trap() /* STUB: not implemented */;
}
};

// A single shard of sharded cache.
class LRUCache {
 public:
  LRUCache();
  ~LRUCache();

  // Separate from constructor so caller can easily make an array of LRUCache
  void SetCapacity(size_t capacity) {
    __builtin_trap() /* STUB: not implemented */;
}

  // Like Cache methods, but with an extra "hash" parameter.
  Cache::Handle* Insert(const Slice& key, uint32_t hash, void* value,
                        size_t charge,
                        void (*deleter)(const Slice& key, void* value));
  Cache::Handle* Lookup(const Slice& key, uint32_t hash);
  void Release(Cache::Handle* handle);
  void Erase(const Slice& key, uint32_t hash);
  void Prune();
  size_t TotalCharge() const {
    __builtin_trap() /* STUB: not implemented */;
}

 private:
  void LRU_Remove(LRUHandle* e);
  void LRU_Append(LRUHandle* list, LRUHandle* e);
  void Ref(LRUHandle* e);
  void Unref(LRUHandle* e);
  bool FinishErase(LRUHandle* e) EXCLUSIVE_LOCKS_REQUIRED(mutex_);

  // Initialized before use.
  size_t capacity_;

  // mutex_ protects the following state.
  mutable port::Mutex mutex_;
  size_t usage_ GUARDED_BY(mutex_);

  // Dummy head of LRU list.
  // lru.prev is newest entry, lru.next is oldest entry.
  // Entries have refs==1 and in_cache==true.
  LRUHandle lru_ GUARDED_BY(mutex_);

  // Dummy head of in-use list.
  // Entries are in use by clients, and have refs >= 2 and in_cache==true.
  LRUHandle in_use_ GUARDED_BY(mutex_);

  HandleTable table_ GUARDED_BY(mutex_);
};

LRUCache::LRUCache() : capacity_(0), usage_(0) {
    __builtin_trap() /* STUB: not implemented */;
}

LRUCache::~LRUCache() {
  assert(in_use_.next == &in_use_);  // Error if caller has an unreleased handle
  for (LRUHandle* e = lru_.next; e != &lru_;) {
    LRUHandle* next = e->next;
    assert(e->in_cache);
    e->in_cache = false;
    assert(e->refs == 1);  // Invariant of lru_ list.
    Unref(e);
    e = next;
  }
}

void LRUCache::Ref(LRUHandle* e) {
    __builtin_trap() /* STUB: not implemented */;
}

void LRUCache::Unref(LRUHandle* e) {
    __builtin_trap() /* STUB: not implemented */;
}

void LRUCache::LRU_Remove(LRUHandle* e) {
    __builtin_trap() /* STUB: not implemented */;
}

void LRUCache::LRU_Append(LRUHandle* list, LRUHandle* e) {
    __builtin_trap() /* STUB: not implemented */;
}

Cache::Handle* LRUCache::Lookup(const Slice& key, uint32_t hash) {
    __builtin_trap() /* STUB: not implemented */;
}

void LRUCache::Release(Cache::Handle* handle) {
    __builtin_trap() /* STUB: not implemented */;
}

Cache::Handle* LRUCache::Insert(const Slice& key, uint32_t hash, void* value,
                                size_t charge,
                                void (*deleter)(const Slice& key,
                                                void* value)) {
    __builtin_trap() /* STUB: not implemented */;
}

// If e != nullptr, finish removing *e from the cache; it has already been
// removed from the hash table.  Return whether e != nullptr.
bool LRUCache::FinishErase(LRUHandle* e) {
    __builtin_trap() /* STUB: not implemented */;
}

void LRUCache::Erase(const Slice& key, uint32_t hash) {
    __builtin_trap() /* STUB: not implemented */;
}

void LRUCache::Prune() {
    __builtin_trap() /* STUB: not implemented */;
}

static const int kNumShardBits = 4;
static const int kNumShards = 1 << kNumShardBits;

class ShardedLRUCache : public Cache {
 private:
  LRUCache shard_[kNumShards];
  port::Mutex id_mutex_;
  uint64_t last_id_;

  static inline uint32_t HashSlice(const Slice& s) {
    __builtin_trap() /* STUB: not implemented */;
}

  static uint32_t Shard(uint32_t hash) {
    __builtin_trap() /* STUB: not implemented */;
}

 public:
  explicit ShardedLRUCache(size_t capacity) : last_id_(0) {
    __builtin_trap() /* STUB: not implemented */;
}
  ~ShardedLRUCache() override {}
  Handle* Insert(const Slice& key, void* value, size_t charge,
                 void (*deleter)(const Slice& key, void* value)) override {
    __builtin_trap() /* STUB: not implemented */;
}
  Handle* Lookup(const Slice& key) override {
    __builtin_trap() /* STUB: not implemented */;
}
  void Release(Handle* handle) override {
    __builtin_trap() /* STUB: not implemented */;
}
  void Erase(const Slice& key) override {
    __builtin_trap() /* STUB: not implemented */;
}
  void* Value(Handle* handle) override {
    __builtin_trap() /* STUB: not implemented */;
}
  uint64_t NewId() override {
    __builtin_trap() /* STUB: not implemented */;
}
  void Prune() override {
    __builtin_trap() /* STUB: not implemented */;
}
  size_t TotalCharge() const override {
    __builtin_trap() /* STUB: not implemented */;
}
};

}  // end anonymous namespace

Cache* NewLRUCache(size_t capacity) {
    __builtin_trap() /* STUB: not implemented */;
}

}  // namespace leveldb
