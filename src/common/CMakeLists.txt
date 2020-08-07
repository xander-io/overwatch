cmake_minimum_required(VERSION 3.14.0)

set(CONTEXT common)
add_library(${CONTEXT} STATIC)

target_sources(${CONTEXT} 
    PRIVATE 
        logging.cpp
        utils.cpp)
target_include_directories(${CONTEXT} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})