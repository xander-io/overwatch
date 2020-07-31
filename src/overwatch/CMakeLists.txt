cmake_minimum_required(VERSION 3.14.0)

# Set the currrent binary name to Overwatch
set(CONTEXT overwatch_${CMAKE_PROJECT_VERSION})
add_executable(${CONTEXT})
target_sources(${CONTEXT}
    PRIVATE
        main.cpp
)
add_subdirectory(overwatch)

target_link_libraries(${CONTEXT} PRIVATE overwatch)
install(TARGETS ${CONTEXT} DESTINATION ${OUTPUT_BIN_DIR})