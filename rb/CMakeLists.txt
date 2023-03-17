enable_testing()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(GCC_FLAGS "-s -Os -W -Wall -Werror")

set(EXECUTABLE_OUTPUT_PATH build)
set(HEADER_FILES src/rb.h)

add_executable(
    rbi
    src/main.cpp
)
