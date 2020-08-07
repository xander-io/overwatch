cmake_minimum_required(VERSION 3.14.0)
project(overwatch
            VERSION 0.0.1)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(OUTPUT_BIN_DIR ${PROJECT_SOURCE_DIR}/bin)
set(EXTERNAL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/include)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
elseif (CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
else()
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W4 /WX")
endif()
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src)
add_subdirectory(tests)