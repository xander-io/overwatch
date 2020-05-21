cmake_minimum_required(VERSION 3.14.0)

# Set the currrent binary name to Overwatch
set(OUTPUT_NAME overwatch_${CMAKE_SYSTEM_NAME}_${CMAKE_BUILD_TYPE})
add_executable(${OUTPUT_NAME})

target_sources(${OUTPUT_NAME}
    PRIVATE
        main.cpp
)
add_subdirectory(liboverwatch)

target_link_libraries(${OUTPUT_NAME} PRIVATE overwatch)
install(TARGETS ${OUTPUT_NAME} DESTINATION ${OUTPUT_BIN_DIR})