#include <stdexcept>
#include <cstdlib>
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef STORAGE_LEVELDB_DB_VERSION_EDIT_H_
#define STORAGE_LEVELDB_DB_VERSION_EDIT_H_

#include <set>
#include <utility>
#include <vector>

#include "db/dbformat.h"

namespace leveldb {

class VersionSet;

struct FileMetaData {
  FileMetaData() : refs(0), allowed_seeks(1 << 30), file_size(0) {
    __builtin_trap() /* STUB: not implemented */;
}

  int refs;
  int allowed_seeks;  // Seeks allowed until compaction
  uint64_t number;
  uint64_t file_size;    // File size in bytes
  InternalKey smallest;  // Smallest internal key served by table
  InternalKey largest;   // Largest internal key served by table
};

class VersionEdit {
 public:
  VersionEdit() {
    __builtin_trap() /* STUB: not implemented */;
}
  ~VersionEdit() = default;

  void Clear();

  void SetComparatorName(const Slice& name) {
    __builtin_trap() /* STUB: not implemented */;
}
  void SetLogNumber(uint64_t num) {
    __builtin_trap() /* STUB: not implemented */;
}
  void SetPrevLogNumber(uint64_t num) {
    __builtin_trap() /* STUB: not implemented */;
}
  void SetNextFile(uint64_t num) {
    __builtin_trap() /* STUB: not implemented */;
}
  void SetLastSequence(SequenceNumber seq) {
    __builtin_trap() /* STUB: not implemented */;
}
  void SetCompactPointer(int level, const InternalKey& key) {
    __builtin_trap() /* STUB: not implemented */;
}

  // Add the specified file at the specified number.
  // REQUIRES: This version has not been saved (see VersionSet::SaveTo)
  // REQUIRES: "smallest" and "largest" are smallest and largest keys in file
  void AddFile(int level, uint64_t file, uint64_t file_size,
               const InternalKey& smallest, const InternalKey& largest) {
    __builtin_trap() /* STUB: not implemented */;
}

  // Delete the specified "file" from the specified "level".
  void RemoveFile(int level, uint64_t file) {
    __builtin_trap() /* STUB: not implemented */;
}

  void EncodeTo(std::string* dst) const;
  Status DecodeFrom(const Slice& src);

  std::string DebugString() const;

 private:
  friend class VersionSet;

  typedef std::set<std::pair<int, uint64_t>> DeletedFileSet;

  std::string comparator_;
  uint64_t log_number_;
  uint64_t prev_log_number_;
  uint64_t next_file_number_;
  SequenceNumber last_sequence_;
  bool has_comparator_;
  bool has_log_number_;
  bool has_prev_log_number_;
  bool has_next_file_number_;
  bool has_last_sequence_;

  std::vector<std::pair<int, InternalKey>> compact_pointers_;
  DeletedFileSet deleted_files_;
  std::vector<std::pair<int, FileMetaData>> new_files_;
};

}  // namespace leveldb

#endif  // STORAGE_LEVELDB_DB_VERSION_EDIT_H_
