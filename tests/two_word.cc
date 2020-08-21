#include "gtest/gtest.h"

#include <string>
#include "../RangedBitset.h"

using std::string;
using namespace ranged_bitset;

// Set tests restricted to the single word
const int size = 128;

TEST(TwoWord, constructor) {
  RangedBitset<size> bitset;
  for (int i = 0; i < size; i++) {
    ASSERT_FALSE(bitset.check(i)) << i;
  }
}

TEST(TwoWord, all) {
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

TEST(TwoWord, bit_by_bit) {
  RangedBitset<size> bitset;
  for (int i = 0; i < size; i++) {
    bitset.set(i, true);
  }
  ASSERT_TRUE(bitset.check_all(0, size - 1, true));
  bitset.invert();
  ASSERT_TRUE(bitset.check_all(0, size - 1, false));
}

TEST(TwoWord, bitwise_or) {
  RangedBitset<size> a;
  RangedBitset<size> b;

  a.set(0, true);
  b.set(1, true);
  a.set(100, true);
  b.set(100, true);
  b.set(101, true);

  a.bitwise_or(b);

  ASSERT_TRUE(a.check(0));
  ASSERT_TRUE(a.check(1));

  ASSERT_TRUE(a.check(100));
  ASSERT_TRUE(a.check(101));

}

TEST(TwoWord, bitwise_and) {
  RangedBitset<size> a;
  RangedBitset<size> b;

  a.set(0, true);
  b.set(1, true);
  a.set(100, true);
  b.set(100, true);
  b.set(101, true);

  a.bitwise_and(b);
  ASSERT_TRUE(a);

  ASSERT_FALSE(a.check(0));
  ASSERT_FALSE(a.check(1));
  ASSERT_TRUE(a.check(100));
  ASSERT_FALSE(a.check(101));
}

TEST(TwoWord, bitwise_clear) {
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
  ASSERT_TRUE(a);
  ASSERT_TRUE(b);

  ASSERT_TRUE(a.check(0));
  ASSERT_TRUE(a.check(2));
  ASSERT_TRUE(a.check(102));

  ASSERT_FALSE(a.check(100));
  ASSERT_FALSE(a.check(101));
}

TEST(TwoWord, invert) {
  RangedBitset<size> a;

  a.set(0, true);
  a.set(1, true);

  a.set(100, true);
  a.set(101, true);

  ASSERT_TRUE(a);
  a.invert();

  ASSERT_FALSE(a.check(0));
  ASSERT_FALSE(a.check(1));
  ASSERT_TRUE(a.check(2));

  ASSERT_FALSE(a.check(100));
  ASSERT_TRUE(a.check(102));
}

TEST(TwoWord, bit) {
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

TEST(TwoWord, ranged_mid_w1_set) {
  RangedBitset<size> a;

  a.set(4, 8, true);

  ASSERT_FALSE(a.check(2));
  ASSERT_FALSE(a.check(3));
  ASSERT_TRUE(a.check(4));
  ASSERT_TRUE(a.check(5));
  ASSERT_TRUE(a.check(6));
  ASSERT_TRUE(a.check(7));
  ASSERT_FALSE(a.check(8));
  ASSERT_FALSE(a.check(9));
}

TEST(TwoWord, ranged_mid_w2_set) {
  RangedBitset<size> a;

  a.set(104, 108, true);

  ASSERT_FALSE(a.check(102));
  ASSERT_FALSE(a.check(103));
  ASSERT_TRUE(a.check(104));
  ASSERT_TRUE(a.check(105));
  ASSERT_TRUE(a.check(106));
  ASSERT_TRUE(a.check(107));
  ASSERT_FALSE(a.check(108));
  ASSERT_FALSE(a.check(109));
}


TEST(TwoWord, ranged_mid_over_boundary) {
  RangedBitset<size> a;
  ASSERT_FALSE(a.check(58));

  a.set(60, 68, true);

  ASSERT_FALSE(a.check(58));
  ASSERT_FALSE(a.check(59));
  ASSERT_TRUE(a.check(60));
  ASSERT_TRUE(a.check(61));
  ASSERT_TRUE(a.check(62));
  ASSERT_TRUE(a.check(63));
  ASSERT_TRUE(a.check(64));
  ASSERT_TRUE(a.check(65));
  ASSERT_TRUE(a.check(66));
  ASSERT_TRUE(a.check(67));
  ASSERT_FALSE(a.check(68));
  ASSERT_FALSE(a.check(69));
  ASSERT_FALSE(a.check(70));
  ASSERT_FALSE(a.check(71));
}

TEST(TwoWord, ranged_mid_over_boundary_clear) {
  RangedBitset<size> a;
  a.invert();
  ASSERT_TRUE(a.check(58));

  a.set(60, 68, false);

  ASSERT_TRUE(a.check(58));
  ASSERT_TRUE(a.check(59));

  ASSERT_FALSE(a.check(60));
  ASSERT_FALSE(a.check(61));
  ASSERT_FALSE(a.check(62));
  ASSERT_FALSE(a.check(63));
  ASSERT_FALSE(a.check(64));
  ASSERT_FALSE(a.check(65));
  ASSERT_FALSE(a.check(66));
  ASSERT_FALSE(a.check(67));

  ASSERT_TRUE(a.check(68));
  ASSERT_TRUE(a.check(69));
  ASSERT_TRUE(a.check(70));
  ASSERT_TRUE(a.check(71));
}

TEST(TwoWord, ranged_mid_over_boundary2) {
  RangedBitset<size> a;
  ASSERT_FALSE(a.check(58));

  a.set(59, 70, true);

  ASSERT_FALSE(a.check(58));
  ASSERT_TRUE(a.check(59));
  ASSERT_TRUE(a.check(60));
  ASSERT_TRUE(a.check(61));
  ASSERT_TRUE(a.check(62));
  ASSERT_TRUE(a.check(63));
  ASSERT_TRUE(a.check(64));
  ASSERT_TRUE(a.check(65));
  ASSERT_TRUE(a.check(66));
  ASSERT_TRUE(a.check(67));
  ASSERT_TRUE(a.check(68));
  ASSERT_TRUE(a.check(69));
  ASSERT_FALSE(a.check(70));
  ASSERT_FALSE(a.check(71));
}

TEST(TwoWord, ranged_mid_3_5_inverted) {
  RangedBitset<size> a;
  a.invert();

  a.set(3, 5, false);

  ASSERT_TRUE(a.check(2));
  ASSERT_FALSE(a.check(3));
  ASSERT_FALSE(a.check(4));
  ASSERT_TRUE(a.check(5));
}

TEST(TwoWord, ranged_mid_0_2) {
  RangedBitset<size> a;

  a.set(0, 2, true);

  ASSERT_TRUE(a.check(0));
  ASSERT_TRUE(a.check(1));
  ASSERT_FALSE(a.check(2));
  ASSERT_FALSE(a.check(3));
}

TEST(TwoWord, ranged_mid_103_105_inverted) {
  RangedBitset<size> a;
  a.invert();

  a.set(103, 105, false);

  ASSERT_TRUE(a.check(102));
  ASSERT_FALSE(a.check(103));
  ASSERT_FALSE(a.check(104));
  ASSERT_TRUE(a.check(105));
}

TEST(TwoWord, empty) {
  RangedBitset<size> a;
  ASSERT_TRUE(a.empty());
  a.set(101, true);
  ASSERT_FALSE(a.empty());
  a.clear();
  ASSERT_FALSE(a);
  a.set(true);
  ASSERT_FALSE(!a);
  a.set(false);
  ASSERT_FALSE(a);
}

TEST(TwoWord, ranged_mid_over_boundary_checkall) {
  RangedBitset<size> a;

  a.set(60, 120, true);

  ASSERT_TRUE(a.check_all(0, 60, false));
  ASSERT_FALSE(a.check_all(0, 120, true));

  ASSERT_TRUE(a.check_all(120, size - 1, false));
  ASSERT_FALSE(a.check_all(120, size - 1, true));
}


