cmake_minimum_required(VERSION 3.14.0)
target_sources(${OUTPUT_NAME}
    PRIVATE
        overwatch_instance.cpp
        arguments_parser.cpp
)
target_include_directories(${OUTPUT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

