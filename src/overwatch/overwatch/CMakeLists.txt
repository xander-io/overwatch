cmake_minimum_required(VERSION 3.14.0)

# Set the current output name to liboverwatch
set(CONTEXT overwatch)
add_library(${CONTEXT} STATIC)

add_subdirectory(core)

# Used in both compiling the target itself and when interfacing with main.cpp
target_include_directories(${CONTEXT} PUBLIC ${EXTERNAL_INCLUDE_DIR})
target_link_libraries(${CONTEXT} PUBLIC common)
