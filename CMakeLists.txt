cmake_minimum_required (VERSION 2.8.11)

project (rbtree_bench)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

add_subdirectory (map)
add_subdirectory (rb)
