cmake_minimum_required(VERSION 3.16)
project(RangedBitset)

set(CMAKE_CXX_STANDARD 17)

find_package(GTest)
set(ranged_bitset_tests
        tests/one_word.cc
        tests/two_word.cc
        tests/three_word.cc
        )

add_library(ranged_bitset STATIC RangedBitset.hpp)
set_target_properties(ranged_bitset PROPERTIES LINKER_LANGUAGE CXX)

if(GTEST_FOUND)
    # only build tests if GTest is available.
    add_executable(ranged_bitset_tests RangedBitset.hpp ${ranged_bitset_tests})
    target_link_libraries(ranged_bitset_tests gtest gtest_main pthread)
    target_include_directories(ranged_bitset_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
endif()


