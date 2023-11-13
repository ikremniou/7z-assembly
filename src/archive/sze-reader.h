#include "../plugin/Export.h"
#include <iterator>

namespace archive {
class ArchiveReader {
 public:
  ArchiveReader(ISequentialInStream* stream) : stream_(stream) {}

  struct Iterator {
    Iterator(ISequentialInStream* stream) : stream_(stream) { read_next(); }

    using iterator_category = std::input_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = char;
    using pointer = char*;
    using reference = char&;

    reference operator*() { return buffer_[current_pos_]; }
    pointer operator->() { return &buffer_[current_pos_]; }
    Iterator& operator++() {
      read_next();
      return *this;
    }
    friend bool operator==(const Iterator& a, const Iterator& b) {
      return a.stream_ == b.stream_;
    }
    friend bool operator!=(const Iterator& a, const Iterator& b) {
      return a.stream_ != b.stream_;
    }

    void read_next() {
      current_pos_++;
      if (stream_ != nullptr && current_pos_ >= extracted_) {
        current_pos_ = 0;
        stream_->Read(buffer_, sizeof(buffer_), &extracted_);
        if (extracted_ == 0) {
          current_pos_ = extracted_ = 0;
          stream_ = nullptr;
        }
      }
    }

   private:
    ISequentialInStream* stream_;
    char buffer_[255];
    UInt32 extracted_ = 0;
    UInt32 current_pos_ = 0;
  };

  Iterator begin() { return Iterator(stream_); }
  Iterator end() { return Iterator(nullptr); }

 private:
  ISequentialInStream* stream_;
};
}