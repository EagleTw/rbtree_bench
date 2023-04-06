enable_testing()

find_package(GTest CONFIG REQUIRED)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_VERBOSE_MAKEFILE TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(GCC_FLAGS "-std=c99-s -O2 -g -W -Wall -Werror -stdlib=libc++")

set(EXECUTABLE_OUTPUT_PATH build)
set(SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/rb.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/rb.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/minibench.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ministat.h
)

#add_library(my_header STATIC
#  ${CMAKE_CURRENT_SOURCE_DIR}/src/rb.h)
add_executable(rb src/main.c)

#set(TEST_SOURCES
#  ${CMAKE_CURRENT_SOURCE_DIR}/src/rb_test.cpp
#)
add_executable(rb_test src/rb_test.c)
target_include_directories(rb_test PRIVATE ${GCC_FLAGS})
target_link_libraries(rb_test PRIVATE  GTest::gtest)
