enable_testing()

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_VERBOSE_MAKEFILE TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(GCC_FLAGS "-std=c99-s -O2 -W -Wall -Werror")

set(EXECUTABLE_OUTPUT_PATH build)
set(
  HEADER_FILES
  src/rb.h
  src/minibench.h
  src/ministat.h
)

add_executable(
    rb
    src/main.c
)
