cmake_minimum_required(VERSION 3.30.3)
project(linear-algebra)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)

# Make sure developers do not run cmake in the main project directory, to keep
# build artifacts from becoming clutter
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
message(FATAL_ERROR "In-source builds not allowed.
Please make a new directory (called a build directory) and run
CMake from there.You may need to remove CMakeCache.txt." )
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(include)

add_library(ThreadPool_lib src/ThreadPool.cpp)

add_executable(main src/main.cpp)

target_link_libraries(
    main
    ThreadPool_lib
)
