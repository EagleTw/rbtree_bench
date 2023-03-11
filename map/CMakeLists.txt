cmake_minimum_required(VERSION 3.5)
project(map)

enable_testing()

find_package(GTest CONFIG REQUIRED)

set(CMAKE_VERBOSE_MAKEFILE TRUE)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(GCC_FLAGS "-s -Os -W -Wall -Werror")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build)
set(HEADER_FILES src/map.h src/map.c)

add_executable(
    map
    src/main.cpp
)
