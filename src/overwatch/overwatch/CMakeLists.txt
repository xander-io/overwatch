cmake_minimum_required(VERSION 3.14.0)

# Set the current output name to liboverwatch
set(CONTEXT overwatch)
add_library(${CONTEXT} STATIC)

add_subdirectory(common)
add_subdirectory(core)