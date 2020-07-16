project(dpp_tests CXX)

enable_testing()

# Configuring executables
add_executable(simple_test simple_test.cpp)

target_include_directories(simple_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(simple_test dpp)
set_target_properties(simple_test PROPERTIES CXX_STANDARD 17)

# Listing tests
add_test(simple_test COMMAND simple_test)
