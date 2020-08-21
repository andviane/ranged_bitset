cmake_minimum_required(VERSION 3.16)
project(RangedBitset)

set(CMAKE_CXX_STANDARD 20)
find_package(GTest)

set(ranged_bitset_tests
        tests/one_word.cc
        tests/two_word.cc
        tests/three_word.cc
)

add_executable(ranged_bitset_tests RangedBitset.h ${ranged_bitset_tests})
target_link_libraries(ranged_bitset_tests gtest gtest_main pthread)
target_include_directories(ranged_bitset_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})



