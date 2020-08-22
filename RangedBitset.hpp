#pragma once

#include <cstring>
#include <cstdint>
#include <cassert>

namespace andviane {
  template<unsigned int SIZE_BITS>
  class RangedBitset {
  public:
    RangedBitset();

    // Set the value of the single bit.
    void set(int bit, bool value);

    // Check the value of the single bit
    bool check(int bit);

    // Set all bits to the given value
    void set(bool value);

    // Set all bits to 0.
    void clear();

    // Set range of bits to the given value
    void set(int from_inclusive, int end_exclusive, bool value);

    // Check if all bits in the range have the same value.
    bool check_all(int from_inclusive, int end_exclusive, bool value) const;

    // Bitwise and operation between this and other set.
    void bitwise_and(const RangedBitset &other);

    // Biwise or operation between this and other set.
    void bitwise_or(const RangedBitset &other);

    // Clear the bits in thist set that are on in the other set.
    void bitwise_clear(const RangedBitset &other);

    // Invert the current set (bitwise not).
    void invert();

    std::string DebugString() const;

    // Access to raw data may be needed for serialization.
    void *data();

    // May be needed for serialization
    size_t size_bytes() const;

    // Overloading some operators for more convenience

    // Checks the needed bit.
    bool operator[](int bit) const;

    // const version (required by assertions in tests)
    // Returns true for a non empty bitset. This is
    // optimized implementation that does not scan all set every time.
    operator bool() const;

    // Returns true for a non empty bitset. This is
    // optimized implementation that does not scan all set every time.
    operator bool();

    // Non operator version
    bool empty();

  private:
    static const unsigned int SIZE_WORDS = SIZE_BITS / 64;
    uint64_t bits_[SIZE_WORDS];

    // Optimization flag that states the set is surely empty.
    bool sure_empty_;

    // Which word does the bit belong
    int word(int bit) const;

    // Which bit does the bit belong
    int bit(int bit) const;
  };

#include "internal/RangedBitset.tpp"
}


