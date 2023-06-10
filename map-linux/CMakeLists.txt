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

add_executable(test-map-linux src/test-map-linux.c ${SOURCES})
add_executable(bench-map-linux-random src/bench-map-linux-random.c ${SOURCES})
add_executable(bench-map-linux-sequential src/bench-map-linux-sequential.c ${SOURCES})
