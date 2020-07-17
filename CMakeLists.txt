cmake_minimum_required(VERSION 3.14)
project(dpp)

option(BUILD_ZLIB OFF)

add_subdirectory(tests)

if (BUILD_ZLIB)
    add_subdirectory(contrib/zlib)
endif ()
file(GLOB SOURCES dpp/*.cpp)

add_library(dpp ${SOURCES} dpp/dpp.hpp dpp/hash_table.hpp)

target_include_directories(dpp
        PUBLIC include
        PUBLIC dpp)

set_target_properties(dpp PROPERTIES CXX_STANDARD 17)

target_compile_options(dpp PUBLIC -msse4.2)
