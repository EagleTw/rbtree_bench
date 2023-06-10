set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_VERBOSE_MAKEFILE TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(GCC_FLAGS "-std=c99-s -O2 -W -Wall -Werror")

#set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE Release)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/map.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/map.c
)

add_executable(test-map-jemalloc src/test-map-jemalloc.c ${SOURCES})
add_executable(bench-map-jemalloc-random src/bench-map-jemalloc-random.c ${SOURCES})
add_executable(bench-map-jemalloc-sequential src/bench-map-jemalloc-sequential.c ${SOURCES})
