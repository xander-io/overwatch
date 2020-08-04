cmake_minimum_required(VERSION 3.14.0)

set(CONTEXT unit_tests)
add_executable(${CONTEXT})

target_sources(${CONTEXT}
    PRIVATE
        main.cpp
)
target_include_directories(${CONTEXT} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(overwatch)

target_include_directories(${CONTEXT} PRIVATE ${EXTERNAL_INCLUDE_DIR})
target_link_libraries(${CONTEXT} PRIVATE overwatch)
install(TARGETS ${CONTEXT} DESTINATION ${OUTPUT_BIN_DIR})