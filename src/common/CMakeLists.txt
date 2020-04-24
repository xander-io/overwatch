cmake_minimum_required(VERSION 3.14.0)

target_sources(${OUTPUT_NAME}
    PRIVATE
        logging.cpp
)

target_include_directories(${OUTPUT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${OUTPUT_NAME} PRIVATE Boost::log Boost::log_setup)