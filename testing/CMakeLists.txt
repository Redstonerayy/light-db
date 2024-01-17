# ------------ project stuff ------------ #
CMAKE_MINIMUM_REQUIRED(VERSION 3.24)
project(lightdbtest)

# ------------ build options ------------ #
set(C_FLAGS "-Wall -std=c17")
set(CXX_FLAGS "-Wall -std=c++20")

find_program(iwyu_path NAMES include-what-you-use iwyu REQUIRED)

# ------------ main executables ------------ #
set(INCLUDE_DIRS ../src/include ./bench)

# ------------ test ------------ #
FILE(GLOB TEST_SOURCE_FILES ../src/database/*.cpp test/*.cpp)
add_executable(test ${TEST_SOURCE_FILES})
target_include_directories(test PRIVATE ${INCLUDE_DIRS})
set_property(TARGET test PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})

# ------------ bench ------------ #
FILE(GLOB BENCH_SOURCE_FILES ../src/database/*.cpp bench/*.cpp test/printing.cpp)
add_executable(bench ${BENCH_SOURCE_FILES})
target_include_directories(bench PRIVATE ${INCLUDE_DIRS})
set_property(TARGET bench PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
