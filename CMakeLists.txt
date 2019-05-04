cmake_minimum_required(VERSION 3.14)
project(deflate__)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCES src/*.cpp)

add_library(deflate ${SOURCES})

target_include_directories(deflate PUBLIC include)
