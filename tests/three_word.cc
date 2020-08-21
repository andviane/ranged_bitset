#include "gtest/gtest.h"

#include <string>
#include "../RangedBitset.hpp"

using std::string;
using namespace andviane;

// Set tests restricted to the single word
const int size = 3*64;

TEST(ThreeWord, constructor) {
  RangedBitset<size> bitset;
  for (int i = 0; i < size; i++) {
    ASSERT_FALSE(bitset.check(i)) << i;
  }
}

TEST(ThreeWord, all) {
  RangedBitset<size> bitset;
  bitset.set(true);

  for (int i = 0; i < size; i++) {
    ASSERT_TRUE(bitset.check(i)) << i;
  }
  ASSERT_TRUE(bitset.check_all(0, size - 1, true));
  ASSERT_FALSE(bitset.check_all(0, size - 1, false));

  bitset.set(false);
  for (int i = 0; i < size; i++) {
    ASSERT_FALSE(bitset.check(i)) << i;
  }

  ASSERT_TRUE(bitset.check_all(0, size - 1, false));
  ASSERT_FALSE(bitset.check_all(0, size - 1, true));
}

TEST(ThreeWord, bit_by_bit) {
  RangedBitset<size> bitset;
  for (int i = 0; i < size; i++) {
    bitset.set(i, true);
  }
  ASSERT_TRUE(bitset.check_all(0, size - 1, true));
  bitset.invert();
  ASSERT_TRUE(bitset.check_all(0, size - 1, false));
}

TEST(ThreeWord, bitwise_or) {
  RangedBitset<size> a;
  RangedBitset<size> b;

  a.set(0, true);
  b.set(1, true);
  a.set(100, true);

  b.set(100, true);
  b.set(101, true);

  a.set(160, true);

  b.set(160, true);
  b.set(161, true);

  a.bitwise_or(b);

  ASSERT_TRUE(a.check(0));
  ASSERT_TRUE(a.check(1));

  ASSERT_TRUE(a.check(100));
  ASSERT_TRUE(a.check(101));

  ASSERT_TRUE(a.check(160));
  ASSERT_TRUE(a.check(161));
}

TEST(ThreeWord, bitwise_and) {
  RangedBitset<size> a;
  RangedBitset<size> b;

  a.set(0, true);
  b.set(1, true);
  a.set(180, true);
  a.set(181, true);
  b.set(180, true);

  a.set(100, true);
  b.set(100, true);
  b.set(101, true);

  a.bitwise_and(b);

  ASSERT_FALSE(a.check(0));
  ASSERT_FALSE(a.check(1));
  ASSERT_TRUE(a.check(100));
  ASSERT_FALSE(a.check(101));

  ASSERT_FALSE(a.check(181));
  ASSERT_TRUE(a.check(180));
}

TEST(ThreeWord, bitwise_clear) {
  RangedBitset<size> a;
  RangedBitset<size> b;

  a.set(0, true);
  b.set(1, true);
  a.set(100, true);
  b.set(100, true);
  b.set(101, true);

  a.set(2, true);
  a.set(102, true);

  a.bitwise_clear(b);

  ASSERT_TRUE(a.check(0));
  ASSERT_TRUE(a.check(2));
  ASSERT_TRUE(a.check(102));

  ASSERT_FALSE(a.check(100));
  ASSERT_FALSE(a.check(101));
}

TEST(ThreeWord, bit) {
  RangedBitset<size> bitset;
  for (int i = 1; i < size; i++) {
    ASSERT_FALSE(bitset.check(i)) << i;
    bitset.set(i, true);
    bitset.set(i - 1, true);
    bitset.set(0, false);
    ASSERT_TRUE(bitset.check(i)) << i;
    bitset.set(i - 1, true);
    bitset.set(0, true);
    ASSERT_TRUE(bitset.check(i)) << i;
    bitset.set(i - 1, false);
    bitset.set(i, false);
    ASSERT_FALSE(bitset.check(i)) << i;
  }
}

TEST(ThreeWord, ranged_mid_over_boundary) {
  RangedBitset<size> a;
  ASSERT_FALSE(a.check(58));

  a.set(60, 180, true);
  for (int bit = 0; bit < 60; bit++) {
    ASSERT_FALSE(a.check(bit)) << bit;
  }
  for (int bit = 60; bit < 180; bit++) {
    ASSERT_TRUE(a.check(bit)) << bit;
  }
  for (int bit = 180; bit < size; bit++) {
    ASSERT_FALSE(a.check(bit)) << bit;
  }
}

TEST(ThreeWord, ranged_mid_over_boundary_checkall) {
  RangedBitset<size> a;

  a.set(60, 180, true);

  ASSERT_TRUE(a.check_all(0, 60, false));
  ASSERT_FALSE(a.check_all(0, 60, true));

  ASSERT_TRUE(a.check_all(60, 180, true));
  ASSERT_FALSE(a.check_all(60, 180, false));

  ASSERT_TRUE(a.check_all(180, size - 1, false));
  ASSERT_FALSE(a.check_all(180, size - 1, true));
}

TEST(ThreeWord, debug_string) {
  RangedBitset<size> a;

  a.set(60, 180, true);
  ASSERT_EQ("000fffffffffffff ffffffffffffffff f000000000000000", a.DebugString());
}


TEST(ThreeWord, ranged_mid_over_boundary_clear) {
  RangedBitset<size> a;
  a.invert();
  ASSERT_TRUE(a.check(58));

  a.set(60, 180, false);

  for (int bit = 0; bit < 60; bit++) {
    ASSERT_TRUE(a.check(bit)) << bit;
  }
  for (int bit = 60; bit < 180; bit++) {
    ASSERT_FALSE(a.check(bit)) << bit;
  }
  for (int bit = 180; bit < size; bit++) {
    ASSERT_TRUE(a.check(bit)) << bit;
  }
}
