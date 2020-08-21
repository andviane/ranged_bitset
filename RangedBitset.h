#pragma once

#include <cstring>
#include <cstdint>
#include <cassert>
#include "RangedBitSetConstants.h"

namespace ranged_bitset {
  template<unsigned int SIZE_BITS>
  class RangedBitset {
  public:
    RangedBitset() : sure_empty_(true) {
      memset(bits_, 0, sizeof(bits_));
    }

    // Set the value of the single bit
    void set(int bit, bool value) {
      if (value) {
        bits_[word(bit)] |= 0x1ll << bit;
        sure_empty_ = false; // Some bits are set
      } else {
        bits_[word(bit)] &= ~(0x1ll << bit);
      }
    }

    // Check the value of the single bit
    bool check(int bit) {
      return bits_[word(bit)] & (0x1ll << bit);
    }

    // Set all bits to the given value
    void set(bool value) {
      if (sure_empty_ && !value) {
        return;
      }
      memset(bits_, value ? ones : zeros, sizeof(bits_));
      sure_empty_ = !value;
    }

    void clear() {
      set(false);
    }

    // Set range of bits to the given value
    void set(int from_inclusive, int end_exclusive, bool value) {
      if (sure_empty_ && !value) {
        return;
      }

      const auto left = word(from_inclusive);
      const auto right = word(end_exclusive - 1);
      assert (left <= right);

      auto midwords = right - left - 1;
      if (midwords > 0) {
        // Set the intermediate range if there is one.
        std::memset(bits_ + (left + 1),
                    value ? 0xFF : 0x00,
                    midwords * sizeof(uint64_t) / sizeof(uint8_t));
      }

      auto &&left_mask = lefts[bit(from_inclusive)];
      auto &&right_mask = lefts[bit(end_exclusive)];

      if (right == left) {
        auto &bits = bits_[left]; // single word
        auto &&mask = left_mask | ~right_mask;
        if (value) {
          bits |= ~mask;
        } else {
          bits &= mask;
        }
      } else {
        if (value) {
          bits_[left] |= ~left_mask;
          bits_[right] |= right_mask;
        } else {
          bits_[left] &= left_mask;
          bits_[right] &= ~right_mask;
        }
      }
      if (value) {
        sure_empty_ = false;
      }
    }

    // Check if all bits in the range have the same value.
    bool check_all(int from_inclusive, int end_exclusive, bool value) const {
      if (!value && sure_empty_) {
        return true;
      }

      const auto left = word(from_inclusive);
      const auto right = word(end_exclusive - 1);
      assert (left <= right);

      const auto v = value ? ones : zeros;
      auto midwords = right - left - 1;

      if (midwords > 0) {
        for (int a = 0; a < midwords; a++) {
          if (bits_[a + 1] != v) {
            return false;
          }
        }
      }

      auto &&left_mask = lefts[bit(from_inclusive)];
      auto &&right_mask = lefts[bit(end_exclusive)];
      bool r = true;

      if (right == left) {
        auto &bits = bits_[left]; // single word
        auto &&mask = left_mask | ~right_mask;
        if (value) {
          r = (bits | mask) == ones;
        } else {
          r = (bits & ~mask) == zeros;
        }
      } else {
        if (value) {
          r = ((bits_[left] | left_mask) == ones) &&
              ((bits_[right] | ~right_mask) == ones);
        } else {
          r = ((bits_[left] & left_mask) == zeros) &&
              ((bits_[right] & ~right_mask) == zeros);
        }
      }
      return r;
    }

    // Bitwise and operation between this and other set.
    void bitwise_and(const RangedBitset &other) {
      if (!sure_empty_) {
        for (auto p = 0; p < SIZE_WORDS; p++) {
          bits_[p] = bits_[p] & other.bits_[p];
        }
      }
    }

    // Biwise or operation between this and other set.
    void bitwise_or(const RangedBitset &other) {
      for (auto p = 0; p < SIZE_WORDS; p++) {
        bits_[p] = bits_[p] | other.bits_[p];
      }
      sure_empty_ = false;
    }

    // Clear the bits in thist set that are on in the other set.
    void bitwise_clear(const RangedBitset &other) {
      if (!sure_empty_) {
        for (auto p = 0; p < SIZE_WORDS; p++) {
          bits_[p] = bits_[p] & ~other.bits_[p];
        }
      }
    }

    // Invert the current set (bitwise not).
    void invert() {
      for (auto p = 0; p < SIZE_WORDS; p++) {
        bits_[p] = ~bits_[p];
      }
      sure_empty_ = false;
    }

    std::string DebugString() const {
      std::string output;
      char buf[20];
      for (int i = SIZE_WORDS - 1; i >= 0; i--) {
        sprintf(buf, "%016lx", bits_[i]);
        output += buf;
        if (i) {
          output += ' ';
        }
      }
      return output;
    }

    // Access to raw data may be needed for serialization.
    void *data() {
      return bits_;
    }

    // May be needed for serialization
    size_t size_bytes() const {
      return sizeof(bits_);
    }

    // Overloading some operators for more convenience

    // Checks the needed bit.
    bool operator[](int bit) const {
      return bits_[word(bit)] & (0x1ll << bit);
    }

    // const version (required by assertions in tests)
    operator bool() const {
      if (sure_empty_) {
        return false;
      }
      for (int a = 0; a < SIZE_WORDS; a++) {
        if (bits_[a]) {
          return true;
        }
      }
      return false;
    }

    operator bool() {
      return !empty(); // caching version
    }

    // Caching version
    bool empty() {
      const RangedBitset &ths = *this;
      sure_empty_ = !ths;
      return sure_empty_;
    }

  private:
    static const unsigned int SIZE_WORDS = SIZE_BITS / 64;
    uint64_t bits_[SIZE_WORDS];

    // Optimization flag that states the set is surely empty.
    bool sure_empty_;

    // Which word does the bit belong
    int word(int bit) const {
      assert (bit >= 0 && bit < SIZE_BITS);
      return bit / 64;
    }

    // Which bit does the bit belong
    int bit(int bit) const {
      assert (bit >= 0 && bit < SIZE_BITS);
      return bit % 64;
    }
  };
}


