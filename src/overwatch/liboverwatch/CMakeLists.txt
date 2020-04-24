cmake_minimum_required(VERSION 3.14.0)

# Set the current output name to liboverwatch
set(OUTPUT_NAME overwatch)
add_library(${OUTPUT_NAME} STATIC)

target_sources(${OUTPUT_NAME} PRIVATE start.cpp)
target_include_directories(${OUTPUT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(common)
add_subdirectory(core)
