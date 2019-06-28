cmake_minimum_required(VERSION 3.14)
project(dpp)

file(GLOB SOURCES src/*.cpp)

add_library(dpp ${SOURCES} include/table.hpp)

target_include_directories(dpp PUBLIC include)

set_target_properties(dpp PROPERTIES CXX_STANDARD 17)
