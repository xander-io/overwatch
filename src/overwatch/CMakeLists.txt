cmake_minimum_required(VERSION 3.14.0)

# Set the currrent binary name to Overwatch
set(CONTEXT overwatch-${CMAKE_BUILD_TYPE})
add_executable(${CONTEXT})
set_property(TARGET ${CONTEXT} PROPERTY POSITION_INDEPENDENT_CODE ON)
target_sources(${CONTEXT}
    PRIVATE
        main.cpp
)
add_subdirectory(overwatch)

target_link_libraries(${CONTEXT} PRIVATE overwatch)
install(TARGETS ${CONTEXT} DESTINATION ${OUTPUT_BIN_DIR})