cmake_minimum_required(VERSION 3.5)
project(pts-red-black-tree)

enable_testing()

find_package(GTest CONFIG REQUIRED)

set (CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(GCC_FLAGS "-DRB_UNORDERED -s -Os -W -Wall -Werror")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build)
set(HEADER_FILES src/rb.h)

add_executable(
    pts-red-black-tree
    src/main.cpp
)
