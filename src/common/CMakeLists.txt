cmake_minimum_required(VERSION 3.14.0)

set(CONTEXT common)
add_library(${CONTEXT} STATIC)

target_sources(${CONTEXT} PRIVATE logging.cpp)
target_include_directories(${CONTEXT} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${CONTEXT} PRIVATE Boost::log Boost::log_setup)