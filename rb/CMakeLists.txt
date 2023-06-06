#enable_testing()
#find_package(GTest CONFIG REQUIRED)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_VERBOSE_MAKEFILE TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(GCC_FLAGS "-std=c99-s -O2 -W -Wall -Werror")

set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_BUILD_TYPE Release)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/map.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/map.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/minibench.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ministat.h
)

add_executable(minibench-rb src/minibench-rb.c ${SOURCES})
add_executable(test-rb src/test-rb.c ${SOURCES})
add_executable(bench-rb src/bench-rb.c ${SOURCES})
target_link_libraries(minibench-rb -lm)
