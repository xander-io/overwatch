cmake_minimum_required(VERSION 3.14.0)
target_sources(${CONTEXT}
    PRIVATE
        arguments_parser.cpp
        config.cpp
)
target_include_directories(${CONTEXT} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${CONTEXT} INTERFACE Boost::program_options)

