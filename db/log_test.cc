#include <stdexcept>
#include <cstdlib>
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "gtest/gtest.h"
#include "db/log_reader.h"
#include "db/log_writer.h"
#include "leveldb/env.h"
#include "util/coding.h"
#include "util/crc32c.h"
#include "util/random.h"

namespace leveldb {
namespace log {

// Construct a string of the specified length made out of the supplied
// partial string.
static std::string BigString(const std::string& partial_string, size_t n) {
  std::string result;
  while (result.size() < n) {
    result.append(partial_string);
  }
  result.resize(n);
  return result;
}

// Construct a string from a number
static std::string NumberString(int n) {
  char buf[50];
  std::snprintf(buf, sizeof(buf), "%d.", n);
  return std::string(buf);
}

// Return a skewed potentially long string
static std::string RandomSkewedString(int i, Random* rnd) {
  return BigString(NumberString(i), rnd->Skewed(17));
}

class LogTest : public testing::Test {
 public:
  LogTest()
      : reading_(false),
        writer_(new Writer(&dest_)),
        reader_(new Reader(&source_, &report_, true /*checksum*/,
                           0 /*initial_offset*/)) {}

  ~LogTest() {
    delete writer_;
    delete reader_;
  }

  void ReopenForAppend() {
    delete writer_;
    writer_ = new Writer(&dest_, dest_.contents_.size());
  }

  void Write(const std::string& msg) {
    ASSERT_TRUE(!reading_) << "Write() after starting to read";
    writer_->AddRecord(Slice(msg));
  }

  size_t WrittenBytes() const { return dest_.contents_.size(); }

  std::string Read() {
    if (!reading_) {
      reading_ = true;
      source_.contents_ = Slice(dest_.contents_);
    }
    std::string scratch;
    Slice record;
    if (reader_->ReadRecord(&record, &scratch)) {
      return record.ToString();
    } else {
      return "EOF";
    }
  }

  void IncrementByte(int offset, int delta) {
    dest_.contents_[offset] += delta;
  }

  void SetByte(int offset, char new_byte) {
    dest_.contents_[offset] = new_byte;
  }

  void ShrinkSize(int bytes) {
    dest_.contents_.resize(dest_.contents_.size() - bytes);
  }

  void FixChecksum(int header_offset, int len) {
    // Compute crc of type/len/data
    uint32_t crc = crc32c::Value(&dest_.contents_[header_offset + 6], 1 + len);
    crc = crc32c::Mask(crc);
    EncodeFixed32(&dest_.contents_[header_offset], crc);
  }

  void ForceError() { source_.force_error_ = true; }

  size_t DroppedBytes() const { return report_.dropped_bytes_; }

  std::string ReportMessage() const { return report_.message_; }

  // Returns OK iff recorded error message contains "msg"
  std::string MatchError(const std::string& msg) const {
    if (report_.message_.find(msg) == std::string::npos) {
      return report_.message_;
    } else {
      return "OK";
    }
  }

  void WriteInitialOffsetLog() {
    for (int i = 0; i < num_initial_offset_records_; i++) {
      std::string record(initial_offset_record_sizes_[i],
                         static_cast<char>('a' + i));
      Write(record);
    }
  }

  void StartReadingAt(uint64_t initial_offset) {
    delete reader_;
    reader_ = new Reader(&source_, &report_, true /*checksum*/, initial_offset);
  }

  void CheckOffsetPastEndReturnsNoRecords(uint64_t offset_past_end) {
    WriteInitialOffsetLog();
    reading_ = true;
    source_.contents_ = Slice(dest_.contents_);
    Reader* offset_reader = new Reader(&source_, &report_, true /*checksum*/,
                                       WrittenBytes() + offset_past_end);
    Slice record;
    std::string scratch;
    ASSERT_TRUE(!offset_reader->ReadRecord(&record, &scratch));
    delete offset_reader;
  }

  void CheckInitialOffsetRecord(uint64_t initial_offset,
                                int expected_record_offset) {
    WriteInitialOffsetLog();
    reading_ = true;
    source_.contents_ = Slice(dest_.contents_);
    Reader* offset_reader =
        new Reader(&source_, &report_, true /*checksum*/, initial_offset);

    // Read all records from expected_record_offset through the last one.
    ASSERT_LT(expected_record_offset, num_initial_offset_records_);
    for (; expected_record_offset < num_initial_offset_records_;
         ++expected_record_offset) {
      Slice record;
      std::string scratch;
      ASSERT_TRUE(offset_reader->ReadRecord(&record, &scratch));
      ASSERT_EQ(initial_offset_record_sizes_[expected_record_offset],
                record.size());
      ASSERT_EQ(initial_offset_last_record_offsets_[expected_record_offset],
                offset_reader->LastRecordOffset());
      ASSERT_EQ((char)('a' + expected_record_offset), record.data()[0]);
    }
    delete offset_reader;
  }

 private:
  class StringDest : public WritableFile {
   public:
    Status Close() override { return Status::OK(); }
    Status Flush() override { return Status::OK(); }
    Status Sync() override { return Status::OK(); }
    Status Append(const Slice& slice) override {
      contents_.append(slice.data(), slice.size());
      return Status::OK();
    }

    std::string contents_;
  };

  class StringSource : public SequentialFile {
   public:
    StringSource() : force_error_(false), returned_partial_(false) {}

    Status Read(size_t n, Slice* result, char* scratch) override {
      EXPECT_TRUE(!returned_partial_) << "must not Read() after eof/error";

      if (force_error_) {
        force_error_ = false;
        returned_partial_ = true;
        return Status::Corruption("read error");
      }

      if (contents_.size() < n) {
        n = contents_.size();
        returned_partial_ = true;
      }
      *result = Slice(contents_.data(), n);
      contents_.remove_prefix(n);
      return Status::OK();
    }

    Status Skip(uint64_t n) override {
      if (n > contents_.size()) {
        contents_.clear();
        return Status::NotFound("in-memory file skipped past end");
      }

      contents_.remove_prefix(n);

      return Status::OK();
    }

    Slice contents_;
    bool force_error_;
    bool returned_partial_;
  };

  class ReportCollector : public Reader::Reporter {
   public:
    ReportCollector() : dropped_bytes_(0) {}
    void Corruption(size_t bytes, const Status& status) override {
      dropped_bytes_ += bytes;
      message_.append(status.ToString());
    }

    size_t dropped_bytes_;
    std::string message_;
  };

  // Record metadata for testing initial offset functionality
  static size_t initial_offset_record_sizes_[];
  static uint64_t initial_offset_last_record_offsets_[];
  static int num_initial_offset_records_;

  StringDest dest_;
  StringSource source_;
  ReportCollector report_;
  bool reading_;
  Writer* writer_;
  Reader* reader_;
};

size_t LogTest::initial_offset_record_sizes_[] = {
    10000,  // Two sizable records in first block
    10000,
    2 * log::kBlockSize - 1000,  // Span three blocks
    1,
    13716,                          // Consume all but two bytes of block 3.
    log::kBlockSize - kHeaderSize,  // Consume the entirety of block 4.
};

uint64_t LogTest::initial_offset_last_record_offsets_[] = {
    0,
    kHeaderSize + 10000,
    2 * (kHeaderSize + 10000),
    2 * (kHeaderSize + 10000) + (2 * log::kBlockSize - 1000) + 3 * kHeaderSize,
    2 * (kHeaderSize + 10000) + (2 * log::kBlockSize - 1000) + 3 * kHeaderSize +
        kHeaderSize + 1,
    3 * log::kBlockSize,
};

// LogTest::initial_offset_last_record_offsets_ must be defined before this.
int LogTest::num_initial_offset_records_ =
    sizeof(LogTest::initial_offset_last_record_offsets_) / sizeof(uint64_t);

TEST_F(LogTest, Empty) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(LogTest, ReadWrite) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(LogTest, ManyBlocks) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(LogTest, Fragmentation) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(LogTest, MarginalTrailer) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(LogTest, MarginalTrailer2) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(LogTest, ShortTrailer) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(LogTest, AlignedEof) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(LogTest, OpenForAppend) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(LogTest, RandomRead) {
    __builtin_trap() /* STUB: not implemented */;
}

// Tests of all the error paths in log_reader.cc follow:

TEST_F(LogTest, ReadError) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(LogTest, BadRecordType) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(LogTest, TruncatedTrailingRecordIsIgnored) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(LogTest, BadLength) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(LogTest, BadLengthAtEndIsIgnored) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(LogTest, ChecksumMismatch) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(LogTest, UnexpectedMiddleType) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(LogTest, UnexpectedLastType) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(LogTest, UnexpectedFullType) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(LogTest, UnexpectedFirstType) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(LogTest, MissingLastIsIgnored) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(LogTest, PartialLastIsIgnored) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(LogTest, SkipIntoMultiRecord) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(LogTest, ErrorJoinsRecords) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(LogTest, ReadStart) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(LogTest, ReadSecondOneOff) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(LogTest, ReadSecondTenThousand) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(LogTest, ReadSecondStart) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(LogTest, ReadThirdOneOff) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(LogTest, ReadThirdStart) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(LogTest, ReadFourthOneOff) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(LogTest, ReadFourthFirstBlockTrailer) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(LogTest, ReadFourthMiddleBlock) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(LogTest, ReadFourthLastBlock) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(LogTest, ReadFourthStart) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(LogTest, ReadInitialOffsetIntoBlockPadding) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(LogTest, ReadEnd) {
    __builtin_trap() /* STUB: not implemented */;
}

TEST_F(LogTest, ReadPastEnd) {
    __builtin_trap() /* STUB: not implemented */;
}

}  // namespace log
}  // namespace leveldb
