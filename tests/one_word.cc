#include "gtest/gtest.h"

#include <string>
#include "../RangedBitset.h"

using std::string;
using namespace ranged_bitset;

// Set tests restricted to the single word
const int size = 64;

TEST(OneWord, constructor) {
  RangedBitset<size> bitset;
  for (int i = 0; i < size; i++) {
    ASSERT_FALSE(bitset.check(i)) << i;
  }
}

TEST(OneWord, all) {
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

TEST(OneWord, bit_by_bit) {
  RangedBitset<size> bitset;
  for (int i = 0; i < size; i++) {
    bitset.set(i, true);
  }
  ASSERT_TRUE(bitset.check_all(0, size - 1, true));
  bitset.invert();
  ASSERT_TRUE(bitset.check_all(0, size - 1, false));
}

TEST(OneWord, bitwise_or) {
  RangedBitset<size> a;
  RangedBitset<size> b;
  RangedBitset<size> c;

  a.set(0, true);
  b.set(1, true);
  a.bitwise_or(b);

  ASSERT_TRUE(a);
  c.bitwise_or(a);
  ASSERT_TRUE(c);

  ASSERT_TRUE(a.check(0));
  ASSERT_TRUE(a.check(1));
  ASSERT_FALSE(a.check(2));
}

TEST(OneWord, bitwise_and) {
  RangedBitset<size> a;
  RangedBitset<size> b;
  RangedBitset<size> c;

  a.set(0, true);
  a.set(1, true);
  b.set(1, true);
  a.bitwise_and(b);

  ASSERT_TRUE(a);
  c.invert();
  c.bitwise_and(b);

  ASSERT_FALSE(a.check(0));
  ASSERT_TRUE(a.check(1));
  ASSERT_FALSE(a.check(2));

  ASSERT_FALSE(c.check(0));
  ASSERT_TRUE(c.check(1));
  ASSERT_FALSE(c.check(2));

  ASSERT_TRUE(c);
  ASSERT_TRUE(b);
}

TEST(OneWord, bitwise_clear) {
  RangedBitset<size> a;
  RangedBitset<size> b;

  a.set(0, true);
  a.set(1, true);
  b.set(1, true);
  a.bitwise_clear(b);

  ASSERT_TRUE(a.check(0));
  ASSERT_FALSE(a.check(1));
  ASSERT_FALSE(a.check(2));
}

TEST(OneWord, invert) {
  RangedBitset<size> a;

  a.set(0, true);
  a.set(1, true);

  a.invert();

  ASSERT_FALSE(a.check(0));
  ASSERT_FALSE(a.check(1));
  ASSERT_TRUE(a.check(2));
}

TEST(OneWord, bit) {
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

TEST(OneWord, ranged_mid) {
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

TEST(OneWord, operator_overloads) {
  RangedBitset<size> a;

  ASSERT_FALSE(a);
  a.set(4, 8, true);
  ASSERT_TRUE(a);

  ASSERT_FALSE(a[2]);
  ASSERT_FALSE(a[3]);
  ASSERT_TRUE(a[4]);
  ASSERT_TRUE(a[5]);
  ASSERT_TRUE(a[6]);
  ASSERT_TRUE(a[7]);
  ASSERT_FALSE(a[8]);
  ASSERT_FALSE(a[9]);
}

TEST(OneWord, ranged_mid_3_5) {
  RangedBitset<size> a;

  a.set(3, 5, true);

  ASSERT_FALSE(a.check(2));
  ASSERT_TRUE(a.check(3));
  ASSERT_TRUE(a.check(4));
  ASSERT_FALSE(a.check(5));
}

TEST(OneWord, ranged_mid_3_5_clear) {
  RangedBitset<size> a;
  a.invert();

  a.set(3, 5, false);

  ASSERT_TRUE(a.check(2));
  ASSERT_FALSE(a.check(3));
  ASSERT_FALSE(a.check(4));
  ASSERT_TRUE(a.check(5));
}

TEST(OneWord, ranged_mid_3_5_inverted) {
  RangedBitset<size> a;
  a.invert();

  a.set(3, 5, false);

  ASSERT_TRUE(a.check(2));
  ASSERT_FALSE(a.check(3));
  ASSERT_FALSE(a.check(4));
  ASSERT_TRUE(a.check(5));
}

TEST(OneWord, ranged_mid_0_2) {
  RangedBitset<size> a;

  a.set(0, 2, true);

  ASSERT_TRUE(a.check(0));
  ASSERT_TRUE(a.check(1));
  ASSERT_FALSE(a.check(2));
  ASSERT_FALSE(a.check(3));
}

TEST(OneWord, ranged_checkall) {
  RangedBitset<size> a;
  ASSERT_FALSE(a);
  a.set(4, 8, true);
  ASSERT_TRUE(a);

  ASSERT_TRUE(a.check_all(0, 4, false));
  ASSERT_FALSE(a.check_all(0, 4, true));

  ASSERT_TRUE(a.check_all(4, 8, true));
  ASSERT_FALSE(a.check_all(4, 8, false));

  ASSERT_TRUE(a.check_all(8, size - 1, false));
  ASSERT_FALSE(a.check_all(8, size - 1, true));
}

TEST(OneWord, extra_checks_sure_empty) {
  RangedBitset<size> a;
  RangedBitset<size> b;

  ASSERT_FALSE(a);
  ASSERT_FALSE(b);

  a.set(1, 2, true);
  ASSERT_TRUE(a);
  a.set(1, 2, false);
  ASSERT_FALSE(a);
  a.set(3, 5, true);
  a.bitwise_and(b);
  ASSERT_FALSE(a);

  a.set(1, 3, true);
  b.bitwise_or(a);
  ASSERT_TRUE(b);
  ASSERT_FALSE(b.empty());
  b.bitwise_clear(a);
  ASSERT_FALSE(b);
  ASSERT_FALSE(!b.empty());
}