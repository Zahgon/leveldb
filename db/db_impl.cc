#include <stdexcept>
#include <cstdlib>
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "db/db_impl.h"

#include <algorithm>
#include <atomic>
#include <cstdint>
#include <cstdio>
#include <set>
#include <string>
#include <vector>

#include "db/builder.h"
#include "db/db_iter.h"
#include "db/dbformat.h"
#include "db/filename.h"
#include "db/log_reader.h"
#include "db/log_writer.h"
#include "db/memtable.h"
#include "db/table_cache.h"
#include "db/version_set.h"
#include "db/write_batch_internal.h"
#include "leveldb/db.h"
#include "leveldb/env.h"
#include "leveldb/status.h"
#include "leveldb/table.h"
#include "leveldb/table_builder.h"
#include "port/port.h"
#include "table/block.h"
#include "table/merger.h"
#include "table/two_level_iterator.h"
#include "util/coding.h"
#include "util/logging.h"
#include "util/mutexlock.h"

namespace leveldb {

const int kNumNonTableCacheFiles = 10;

// Information kept for every waiting writer
struct DBImpl::Writer {
  explicit Writer(port::Mutex* mu)
      : batch(nullptr), sync(false), done(false), cv(mu) {
    __builtin_trap() /* STUB: not implemented */;
}

  Status status;
  WriteBatch* batch;
  bool sync;
  bool done;
  port::CondVar cv;
};

struct DBImpl::CompactionState {
  // Files produced by compaction
  struct Output {
    uint64_t number;
    uint64_t file_size;
    InternalKey smallest, largest;
  };

  Output* current_output() {
    __builtin_trap() /* STUB: not implemented */;
}

  explicit CompactionState(Compaction* c)
      : compaction(c),
        smallest_snapshot(0),
        outfile(nullptr),
        builder(nullptr),
        total_bytes(0) {
    __builtin_trap() /* STUB: not implemented */;
}

  Compaction* const compaction;

  // Sequence numbers < smallest_snapshot are not significant since we
  // will never have to service a snapshot below smallest_snapshot.
  // Therefore if we have seen a sequence number S <= smallest_snapshot,
  // we can drop all entries for the same key with sequence numbers < S.
  SequenceNumber smallest_snapshot;

  std::vector<Output> outputs;

  // State kept for output being generated
  WritableFile* outfile;
  TableBuilder* builder;

  uint64_t total_bytes;
};

// Fix user-supplied options to be reasonable
template <class T, class V>
static void ClipToRange(T* ptr, V minvalue, V maxvalue) {
    __builtin_trap() /* STUB: not implemented */;
}
Options SanitizeOptions(const std::string& dbname,
                        const InternalKeyComparator* icmp,
                        const InternalFilterPolicy* ipolicy,
                        const Options& src) {
    __builtin_trap() /* STUB: not implemented */;
}

static int TableCacheSize(const Options& sanitized_options) {
    __builtin_trap() /* STUB: not implemented */;
}

DBImpl::DBImpl(const Options& raw_options, const std::string& dbname)
    : env_(raw_options.env),
      internal_comparator_(raw_options.comparator),
      internal_filter_policy_(raw_options.filter_policy),
      options_(SanitizeOptions(dbname, &internal_comparator_,
                               &internal_filter_policy_, raw_options)),
      owns_info_log_(options_.info_log != raw_options.info_log),
      owns_cache_(options_.block_cache != raw_options.block_cache),
      dbname_(dbname),
      table_cache_(new TableCache(dbname_, options_, TableCacheSize(options_))),
      db_lock_(nullptr),
      shutting_down_(false),
      background_work_finished_signal_(&mutex_),
      mem_(nullptr),
      imm_(nullptr),
      has_imm_(false),
      logfile_(nullptr),
      logfile_number_(0),
      log_(nullptr),
      seed_(0),
      tmp_batch_(new WriteBatch),
      background_compaction_scheduled_(false),
      manual_compaction_(nullptr),
      versions_(new VersionSet(dbname_, &options_, table_cache_,
                               &internal_comparator_)) {
    __builtin_trap() /* STUB: not implemented */;
}

DBImpl::~DBImpl() {
  // Wait for background work to finish.
  mutex_.Lock();
  shutting_down_.store(true, std::memory_order_release);
  while (background_compaction_scheduled_) {
    background_work_finished_signal_.Wait();
  }
  mutex_.Unlock();

  if (db_lock_ != nullptr) {
    env_->UnlockFile(db_lock_);
  }

  delete versions_;
  if (mem_ != nullptr) mem_->Unref();
  if (imm_ != nullptr) imm_->Unref();
  delete tmp_batch_;
  delete log_;
  delete logfile_;
  delete table_cache_;

  if (owns_info_log_) {
    delete options_.info_log;
  }
  if (owns_cache_) {
    delete options_.block_cache;
  }
}

Status DBImpl::NewDB() {
    __builtin_trap() /* STUB: not implemented */;
}

void DBImpl::MaybeIgnoreError(Status* s) const {
    __builtin_trap() /* STUB: not implemented */;
}

void DBImpl::RemoveObsoleteFiles() {
    __builtin_trap() /* STUB: not implemented */;
}

Status DBImpl::Recover(VersionEdit* edit, bool* save_manifest) {
    __builtin_trap() /* STUB: not implemented */;
}

Status DBImpl::RecoverLogFile(uint64_t log_number, bool last_log,
                              bool* save_manifest, VersionEdit* edit,
                              SequenceNumber* max_sequence) {
    __builtin_trap() /* STUB: not implemented */;
}

Status DBImpl::WriteLevel0Table(MemTable* mem, VersionEdit* edit,
                                Version* base) {
    __builtin_trap() /* STUB: not implemented */;
}

void DBImpl::CompactMemTable() {
    __builtin_trap() /* STUB: not implemented */;
}

void DBImpl::CompactRange(const Slice* begin, const Slice* end) {
    __builtin_trap() /* STUB: not implemented */;
}

void DBImpl::TEST_CompactRange(int level, const Slice* begin,
                               const Slice* end) {
  assert(level >= 0);
  assert(level + 1 < config::kNumLevels);

  InternalKey begin_storage, end_storage;

  ManualCompaction manual;
  manual.level = level;
  manual.done = false;
  if (begin == nullptr) {
    manual.begin = nullptr;
  } else {
    begin_storage = InternalKey(*begin, kMaxSequenceNumber, kValueTypeForSeek);
    manual.begin = &begin_storage;
  }
  if (end == nullptr) {
    manual.end = nullptr;
  } else {
    end_storage = InternalKey(*end, 0, static_cast<ValueType>(0));
    manual.end = &end_storage;
  }

  MutexLock l(&mutex_);
  while (!manual.done && !shutting_down_.load(std::memory_order_acquire) &&
         bg_error_.ok()) {
    if (manual_compaction_ == nullptr) {  // Idle
      manual_compaction_ = &manual;
      MaybeScheduleCompaction();
    } else {  // Running either my compaction or another compaction.
      background_work_finished_signal_.Wait();
    }
  }
  // Finish current background compaction in the case where
  // `background_work_finished_signal_` was signalled due to an error.
  while (background_compaction_scheduled_) {
    background_work_finished_signal_.Wait();
  }
  if (manual_compaction_ == &manual) {
    // Cancel my manual compaction since we aborted early for some reason.
    manual_compaction_ = nullptr;
  }
}

Status DBImpl::TEST_CompactMemTable() {
  // nullptr batch means just wait for earlier writes to be done
  Status s = Write(WriteOptions(), nullptr);
  if (s.ok()) {
    // Wait until the compaction completes
    MutexLock l(&mutex_);
    while (imm_ != nullptr && bg_error_.ok() &&
           !shutting_down_.load(std::memory_order_acquire)) {
      background_work_finished_signal_.Wait();
    }
    if (imm_ != nullptr) {
      s = bg_error_;
    }
  }
  return s;
}

void DBImpl::RecordBackgroundError(const Status& s) {
    __builtin_trap() /* STUB: not implemented */;
}

void DBImpl::MaybeScheduleCompaction() {
    __builtin_trap() /* STUB: not implemented */;
}

void DBImpl::BGWork(void* db) {
    __builtin_trap() /* STUB: not implemented */;
}

void DBImpl::BackgroundCall() {
    __builtin_trap() /* STUB: not implemented */;
}

void DBImpl::BackgroundCompaction() {
    __builtin_trap() /* STUB: not implemented */;
}

void DBImpl::CleanupCompaction(CompactionState* compact) {
    __builtin_trap() /* STUB: not implemented */;
}

Status DBImpl::OpenCompactionOutputFile(CompactionState* compact) {
    __builtin_trap() /* STUB: not implemented */;
}

Status DBImpl::FinishCompactionOutputFile(CompactionState* compact,
                                          Iterator* input) {
    __builtin_trap() /* STUB: not implemented */;
}

Status DBImpl::InstallCompactionResults(CompactionState* compact) {
    __builtin_trap() /* STUB: not implemented */;
}

Status DBImpl::DoCompactionWork(CompactionState* compact) {
    __builtin_trap() /* STUB: not implemented */;
}

namespace {

struct IterState {
  port::Mutex* const mu;
  Version* const version GUARDED_BY(mu);
  MemTable* const mem GUARDED_BY(mu);
  MemTable* const imm GUARDED_BY(mu);

  IterState(port::Mutex* mutex, MemTable* mem, MemTable* imm, Version* version)
      : mu(mutex), version(version), mem(mem), imm(imm) {
    __builtin_trap() /* STUB: not implemented */;
}
};

static void CleanupIteratorState(void* arg1, void* arg2) {
    __builtin_trap() /* STUB: not implemented */;
}

}  // anonymous namespace

Iterator* DBImpl::NewInternalIterator(const ReadOptions& options,
                                      SequenceNumber* latest_snapshot,
                                      uint32_t* seed) {
    __builtin_trap() /* STUB: not implemented */;
}

Iterator* DBImpl::TEST_NewInternalIterator() {
  SequenceNumber ignored;
  uint32_t ignored_seed;
  return NewInternalIterator(ReadOptions(), &ignored, &ignored_seed);
}

int64_t DBImpl::TEST_MaxNextLevelOverlappingBytes() {
  MutexLock l(&mutex_);
  return versions_->MaxNextLevelOverlappingBytes();
}

Status DBImpl::Get(const ReadOptions& options, const Slice& key,
                   std::string* value) {
    __builtin_trap() /* STUB: not implemented */;
}

Iterator* DBImpl::NewIterator(const ReadOptions& options) {
    __builtin_trap() /* STUB: not implemented */;
}

void DBImpl::RecordReadSample(Slice key) {
    __builtin_trap() /* STUB: not implemented */;
}

const Snapshot* DBImpl::GetSnapshot() {
    __builtin_trap() /* STUB: not implemented */;
}

void DBImpl::ReleaseSnapshot(const Snapshot* snapshot) {
    __builtin_trap() /* STUB: not implemented */;
}

// Convenience methods
Status DBImpl::Put(const WriteOptions& o, const Slice& key, const Slice& val) {
    __builtin_trap() /* STUB: not implemented */;
}

Status DBImpl::Delete(const WriteOptions& options, const Slice& key) {
    __builtin_trap() /* STUB: not implemented */;
}

Status DBImpl::Write(const WriteOptions& options, WriteBatch* updates) {
    __builtin_trap() /* STUB: not implemented */;
}

// REQUIRES: Writer list must be non-empty
// REQUIRES: First writer must have a non-null batch
WriteBatch* DBImpl::BuildBatchGroup(Writer** last_writer) {
    __builtin_trap() /* STUB: not implemented */;
}

// REQUIRES: mutex_ is held
// REQUIRES: this thread is currently at the front of the writer queue
Status DBImpl::MakeRoomForWrite(bool force) {
    __builtin_trap() /* STUB: not implemented */;
}

bool DBImpl::GetProperty(const Slice& property, std::string* value) {
    __builtin_trap() /* STUB: not implemented */;
}

void DBImpl::GetApproximateSizes(const Range* range, int n, uint64_t* sizes) {
    __builtin_trap() /* STUB: not implemented */;
}

// Default implementations of convenience methods that subclasses of DB
// can call if they wish
Status DB::Put(const WriteOptions& opt, const Slice& key, const Slice& value) {
  WriteBatch batch;
  batch.Put(key, value);
  return Write(opt, &batch);
}

Status DB::Delete(const WriteOptions& opt, const Slice& key) {
  WriteBatch batch;
  batch.Delete(key);
  return Write(opt, &batch);
}

DB::~DB() = default;

Status DB::Open(const Options& options, const std::string& dbname, DB** dbptr) {
    __builtin_trap() /* STUB: not implemented */;
}

Snapshot::~Snapshot() = default;

Status DestroyDB(const std::string& dbname, const Options& options) {
    __builtin_trap() /* STUB: not implemented */;
}

}  // namespace leveldb
