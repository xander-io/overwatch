cmake_minimum_required(VERSION 3.14.0)
target_sources(${CONTEXT}
    PRIVATE
        config.cpp
        argument_parser.cpp
)

target_include_directories(${CONTEXT} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
