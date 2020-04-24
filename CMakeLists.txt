cmake_minimum_required(VERSION 3.14.0)
project(overwatch)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.69.0 COMPONENTS program_options log log_setup REQUIRED) 

set(OUTPUT_BIN_DIR ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src)
#add_subdirectory(test)