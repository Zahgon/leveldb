#include <stdexcept>
#include <cstdlib>
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "db/version_set.h"

#include <algorithm>
#include <cstdio>

#include "db/filename.h"
#include "db/log_reader.h"
#include "db/log_writer.h"
#include "db/memtable.h"
#include "db/table_cache.h"
#include "leveldb/env.h"
#include "leveldb/table_builder.h"
#include "table/merger.h"
#include "table/two_level_iterator.h"
#include "util/coding.h"
#include "util/logging.h"

namespace leveldb {

static size_t TargetFileSize(const Options* options) {
    __builtin_trap() /* STUB: not implemented */;
}

// Maximum bytes of overlaps in grandparent (i.e., level+2) before we
// stop building a single file in a level->level+1 compaction.
static int64_t MaxGrandParentOverlapBytes(const Options* options) {
    __builtin_trap() /* STUB: not implemented */;
}

// Maximum number of bytes in all compacted files.  We avoid expanding
// the lower level file set of a compaction if it would make the
// total compaction cover more than this many bytes.
static int64_t ExpandedCompactionByteSizeLimit(const Options* options) {
    __builtin_trap() /* STUB: not implemented */;
}

static double MaxBytesForLevel(const Options* options, int level) {
    __builtin_trap() /* STUB: not implemented */;
}

static uint64_t MaxFileSizeForLevel(const Options* options, int level) {
    __builtin_trap() /* STUB: not implemented */;
}

static int64_t TotalFileSize(const std::vector<FileMetaData*>& files) {
    __builtin_trap() /* STUB: not implemented */;
}

Version::~Version() {
  assert(refs_ == 0);

  // Remove from linked list
  prev_->next_ = next_;
  next_->prev_ = prev_;

  // Drop references to files
  for (int level = 0; level < config::kNumLevels; level++) {
    for (size_t i = 0; i < files_[level].size(); i++) {
      FileMetaData* f = files_[level][i];
      assert(f->refs > 0);
      f->refs--;
      if (f->refs <= 0) {
        delete f;
      }
    }
  }
}

int FindFile(const InternalKeyComparator& icmp,
             const std::vector<FileMetaData*>& files, const Slice& key) {
    __builtin_trap() /* STUB: not implemented */;
}

static bool AfterFile(const Comparator* ucmp, const Slice* user_key,
                      const FileMetaData* f) {
    __builtin_trap() /* STUB: not implemented */;
}

static bool BeforeFile(const Comparator* ucmp, const Slice* user_key,
                       const FileMetaData* f) {
    __builtin_trap() /* STUB: not implemented */;
}

bool SomeFileOverlapsRange(const InternalKeyComparator& icmp,
                           bool disjoint_sorted_files,
                           const std::vector<FileMetaData*>& files,
                           const Slice* smallest_user_key,
                           const Slice* largest_user_key) {
    __builtin_trap() /* STUB: not implemented */;
}

// An internal iterator.  For a given version/level pair, yields
// information about the files in the level.  For a given entry, key()
// is the largest key that occurs in the file, and value() is an
// 16-byte value containing the file number and file size, both
// encoded using EncodeFixed64.
class Version::LevelFileNumIterator : public Iterator {
 public:
  LevelFileNumIterator(const InternalKeyComparator& icmp,
                       const std::vector<FileMetaData*>* flist)
      : icmp_(icmp), flist_(flist), index_(flist->size()) {
    __builtin_trap() /* STUB: not implemented */;
}
  bool Valid() const override {
    __builtin_trap() /* STUB: not implemented */;
}
  void Seek(const Slice& target) override {
    __builtin_trap() /* STUB: not implemented */;
}
  void SeekToFirst() override {
    __builtin_trap() /* STUB: not implemented */;
}
  void SeekToLast() override {
    __builtin_trap() /* STUB: not implemented */;
}
  void Next() override {
    __builtin_trap() /* STUB: not implemented */;
}
  void Prev() override {
    __builtin_trap() /* STUB: not implemented */;
}
  Slice key() const override {
    __builtin_trap() /* STUB: not implemented */;
}
  Slice value() const override {
    __builtin_trap() /* STUB: not implemented */;
}
  Status status() const override {
    __builtin_trap() /* STUB: not implemented */;
}

 private:
  const InternalKeyComparator icmp_;
  const std::vector<FileMetaData*>* const flist_;
  uint32_t index_;

  // Backing store for value().  Holds the file number and size.
  mutable char value_buf_[16];
};

static Iterator* GetFileIterator(void* arg, const ReadOptions& options,
                                 const Slice& file_value) {
    __builtin_trap() /* STUB: not implemented */;
}

Iterator* Version::NewConcatenatingIterator(const ReadOptions& options,
                                            int level) const {
    __builtin_trap() /* STUB: not implemented */;
}

void Version::AddIterators(const ReadOptions& options,
                           std::vector<Iterator*>* iters) {
    __builtin_trap() /* STUB: not implemented */;
}

// Callback from TableCache::Get()
namespace {
enum SaverState {
  kNotFound,
  kFound,
  kDeleted,
  kCorrupt,
};
struct Saver {
  SaverState state;
  const Comparator* ucmp;
  Slice user_key;
  std::string* value;
};
}  // namespace
static void SaveValue(void* arg, const Slice& ikey, const Slice& v) {
    __builtin_trap() /* STUB: not implemented */;
}

static bool NewestFirst(FileMetaData* a, FileMetaData* b) {
    __builtin_trap() /* STUB: not implemented */;
}

void Version::ForEachOverlapping(Slice user_key, Slice internal_key, void* arg,
                                 bool (*func)(void*, int, FileMetaData*)) {
    __builtin_trap() /* STUB: not implemented */;
}

Status Version::Get(const ReadOptions& options, const LookupKey& k,
                    std::string* value, GetStats* stats) {
    __builtin_trap() /* STUB: not implemented */;
}

bool Version::UpdateStats(const GetStats& stats) {
    __builtin_trap() /* STUB: not implemented */;
}

bool Version::RecordReadSample(Slice internal_key) {
    __builtin_trap() /* STUB: not implemented */;
}

void Version::Ref() {
    __builtin_trap() /* STUB: not implemented */;
}

void Version::Unref() {
    __builtin_trap() /* STUB: not implemented */;
}

bool Version::OverlapInLevel(int level, const Slice* smallest_user_key,
                             const Slice* largest_user_key) {
    __builtin_trap() /* STUB: not implemented */;
}

int Version::PickLevelForMemTableOutput(const Slice& smallest_user_key,
                                        const Slice& largest_user_key) {
    __builtin_trap() /* STUB: not implemented */;
}

// Store in "*inputs" all files in "level" that overlap [begin,end]
void Version::GetOverlappingInputs(int level, const InternalKey* begin,
                                   const InternalKey* end,
                                   std::vector<FileMetaData*>* inputs) {
    __builtin_trap() /* STUB: not implemented */;
}

std::string Version::DebugString() const {
    __builtin_trap() /* STUB: not implemented */;
}

// A helper class so we can efficiently apply a whole sequence
// of edits to a particular state without creating intermediate
// Versions that contain full copies of the intermediate state.
class VersionSet::Builder {
 private:
  // Helper to sort by v->files_[file_number].smallest
  struct BySmallestKey {
    const InternalKeyComparator* internal_comparator;

    bool operator()(FileMetaData* f1, FileMetaData* f2) const {
    __builtin_trap() /* STUB: not implemented */;
}
  };

  typedef std::set<FileMetaData*, BySmallestKey> FileSet;
  struct LevelState {
    std::set<uint64_t> deleted_files;
    FileSet* added_files;
  };

  VersionSet* vset_;
  Version* base_;
  LevelState levels_[config::kNumLevels];

 public:
  // Initialize a builder with the files from *base and other info from *vset
  Builder(VersionSet* vset, Version* base) : vset_(vset), base_(base) {
    __builtin_trap() /* STUB: not implemented */;
}

  ~Builder() {
    for (int level = 0; level < config::kNumLevels; level++) {
      const FileSet* added = levels_[level].added_files;
      std::vector<FileMetaData*> to_unref;
      to_unref.reserve(added->size());
      for (FileSet::const_iterator it = added->begin(); it != added->end();
           ++it) {
        to_unref.push_back(*it);
      }
      delete added;
      for (uint32_t i = 0; i < to_unref.size(); i++) {
        FileMetaData* f = to_unref[i];
        f->refs--;
        if (f->refs <= 0) {
          delete f;
        }
      }
    }
    base_->Unref();
  }

  // Apply all of the edits in *edit to the current state.
  void Apply(const VersionEdit* edit) {
    __builtin_trap() /* STUB: not implemented */;
}

  // Save the current state in *v.
  void SaveTo(Version* v) {
    __builtin_trap() /* STUB: not implemented */;
}

  void MaybeAddFile(Version* v, int level, FileMetaData* f) {
    __builtin_trap() /* STUB: not implemented */;
}
};

VersionSet::VersionSet(const std::string& dbname, const Options* options,
                       TableCache* table_cache,
                       const InternalKeyComparator* cmp)
    : env_(options->env),
      dbname_(dbname),
      options_(options),
      table_cache_(table_cache),
      icmp_(*cmp),
      next_file_number_(2),
      manifest_file_number_(0),  // Filled by Recover()
      last_sequence_(0),
      log_number_(0),
      prev_log_number_(0),
      descriptor_file_(nullptr),
      descriptor_log_(nullptr),
      dummy_versions_(this),
      current_(nullptr) {
    __builtin_trap() /* STUB: not implemented */;
}

VersionSet::~VersionSet() {
  current_->Unref();
  assert(dummy_versions_.next_ == &dummy_versions_);  // List must be empty
  delete descriptor_log_;
  delete descriptor_file_;
}

void VersionSet::AppendVersion(Version* v) {
    __builtin_trap() /* STUB: not implemented */;
}

Status VersionSet::LogAndApply(VersionEdit* edit, port::Mutex* mu) {
    __builtin_trap() /* STUB: not implemented */;
}

Status VersionSet::Recover(bool* save_manifest) {
    __builtin_trap() /* STUB: not implemented */;
}

bool VersionSet::ReuseManifest(const std::string& dscname,
                               const std::string& dscbase) {
    __builtin_trap() /* STUB: not implemented */;
}

void VersionSet::MarkFileNumberUsed(uint64_t number) {
    __builtin_trap() /* STUB: not implemented */;
}

void VersionSet::Finalize(Version* v) {
    __builtin_trap() /* STUB: not implemented */;
}

Status VersionSet::WriteSnapshot(log::Writer* log) {
    __builtin_trap() /* STUB: not implemented */;
}

int VersionSet::NumLevelFiles(int level) const {
    __builtin_trap() /* STUB: not implemented */;
}

const char* VersionSet::LevelSummary(LevelSummaryStorage* scratch) const {
    __builtin_trap() /* STUB: not implemented */;
}

uint64_t VersionSet::ApproximateOffsetOf(Version* v, const InternalKey& ikey) {
    __builtin_trap() /* STUB: not implemented */;
}

void VersionSet::AddLiveFiles(std::set<uint64_t>* live) {
    __builtin_trap() /* STUB: not implemented */;
}

int64_t VersionSet::NumLevelBytes(int level) const {
    __builtin_trap() /* STUB: not implemented */;
}

int64_t VersionSet::MaxNextLevelOverlappingBytes() {
    __builtin_trap() /* STUB: not implemented */;
}

// Stores the minimal range that covers all entries in inputs in
// *smallest, *largest.
// REQUIRES: inputs is not empty
void VersionSet::GetRange(const std::vector<FileMetaData*>& inputs,
                          InternalKey* smallest, InternalKey* largest) {
    __builtin_trap() /* STUB: not implemented */;
}

// Stores the minimal range that covers all entries in inputs1 and inputs2
// in *smallest, *largest.
// REQUIRES: inputs is not empty
void VersionSet::GetRange2(const std::vector<FileMetaData*>& inputs1,
                           const std::vector<FileMetaData*>& inputs2,
                           InternalKey* smallest, InternalKey* largest) {
    __builtin_trap() /* STUB: not implemented */;
}

Iterator* VersionSet::MakeInputIterator(Compaction* c) {
    __builtin_trap() /* STUB: not implemented */;
}

Compaction* VersionSet::PickCompaction() {
    __builtin_trap() /* STUB: not implemented */;
}

// Finds the largest key in a vector of files. Returns true if files is not
// empty.
bool FindLargestKey(const InternalKeyComparator& icmp,
                    const std::vector<FileMetaData*>& files,
                    InternalKey* largest_key) {
    __builtin_trap() /* STUB: not implemented */;
}

// Finds minimum file b2=(l2, u2) in level file for which l2 > u1 and
// user_key(l2) = user_key(u1)
FileMetaData* FindSmallestBoundaryFile(
    const InternalKeyComparator& icmp,
    const std::vector<FileMetaData*>& level_files,
    const InternalKey& largest_key) {
    __builtin_trap() /* STUB: not implemented */;
}

// Extracts the largest file b1 from |compaction_files| and then searches for a
// b2 in |level_files| for which user_key(u1) = user_key(l2). If it finds such a
// file b2 (known as a boundary file) it adds it to |compaction_files| and then
// searches again using this new upper bound.
//
// If there are two blocks, b1=(l1, u1) and b2=(l2, u2) and
// user_key(u1) = user_key(l2), and if we compact b1 but not b2 then a
// subsequent get operation will yield an incorrect result because it will
// return the record from b2 in level i rather than from b1 because it searches
// level by level for records matching the supplied user key.
//
// parameters:
//   in     level_files:      List of files to search for boundary files.
//   in/out compaction_files: List of files to extend by adding boundary files.
void AddBoundaryInputs(const InternalKeyComparator& icmp,
                       const std::vector<FileMetaData*>& level_files,
                       std::vector<FileMetaData*>* compaction_files) {
    __builtin_trap() /* STUB: not implemented */;
}

void VersionSet::SetupOtherInputs(Compaction* c) {
    __builtin_trap() /* STUB: not implemented */;
}

Compaction* VersionSet::CompactRange(int level, const InternalKey* begin,
                                     const InternalKey* end) {
    __builtin_trap() /* STUB: not implemented */;
}

Compaction::Compaction(const Options* options, int level)
    : level_(level),
      max_output_file_size_(MaxFileSizeForLevel(options, level)),
      input_version_(nullptr),
      grandparent_index_(0),
      seen_key_(false),
      overlapped_bytes_(0) {
    __builtin_trap() /* STUB: not implemented */;
}

Compaction::~Compaction() {
  if (input_version_ != nullptr) {
    input_version_->Unref();
  }
}

bool Compaction::IsTrivialMove() const {
    __builtin_trap() /* STUB: not implemented */;
}

void Compaction::AddInputDeletions(VersionEdit* edit) {
    __builtin_trap() /* STUB: not implemented */;
}

bool Compaction::IsBaseLevelForKey(const Slice& user_key) {
    __builtin_trap() /* STUB: not implemented */;
}

bool Compaction::ShouldStopBefore(const Slice& internal_key) {
    __builtin_trap() /* STUB: not implemented */;
}

void Compaction::ReleaseInputs() {
    __builtin_trap() /* STUB: not implemented */;
}

}  // namespace leveldb
