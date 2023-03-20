enable_testing()

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_VERBOSE_MAKEFILE TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(GCC_FLAGS "-std=c99-s -Os -W -Wall -Werror")

set(EXECUTABLE_OUTPUT_PATH build)
set(HEADER_FILES
  src/map.h
  src/map.c
  src/minibench.h
  src/ministat.h
)

add_executable(
    map
    src/main.c
)
