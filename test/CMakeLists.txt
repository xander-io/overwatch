cmake_minimum_required(VERSION 3.14.0)

set(OUTPUT_NAME unit_tests_catch)
add_executable(${OUTPUT_NAME})

add_subdirectory(catch)

target_link_libraries(${JARVIS_TEST_NAME} PRIVATE Catch2)

install(TARGETS ${OUTPUT_NAME} DESTINATION ${OUTPUT_BIN_DIR})