cmake_minimum_required(VERSION 3.14)
project(dpp)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCES src/*.cpp)

add_library(dpp ${SOURCES} include/interfaces/table.hpp)

target_include_directories(dpp PUBLIC include)
