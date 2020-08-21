# Ranged bitset implementation in C++

This project provides a bitset that can work with ranges. This is something that does not come with STL bitset, yet it happened
several times that it was required. 

Range boundaries are provides as two integers, start inclusive and end exclusive.
Internally the set uses an array of uint64_t to store the values, one value per bit.
The total size of the bitset is only limited by memory.

```  
  using namespace andviane;

  // 16 bit bitset (size is template paramter)
  RangedBitset<16> a; 

  // Setting bits:
  a.set(4, 8, true); // range
  a.set(2, true); // single bit

  // Checking bits 
  bool is = a.check(2); // check the bit
  bool is_ = a[2]; // check the bit, alternative
  bool is_all_range = a.check(2, 6, true); // check the range
  bool not_empty = a;

  // Bitwise operations
  a.bitwise_or(other_set); 
  a.bitwise_and(other_set);
  a.bitwise_clear(other_set);

  // All set operations
  a.clear(); // clear all
  a.set(true); // set all
  a.invert();  
```

The set is coming with the test suite to ensure operations are done as expected.



