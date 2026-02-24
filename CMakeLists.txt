cmake_minimum_required(VERSION 3.22)
project(nbody LANGUAGES CXX)

# Use C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Generate compile_commands.json for LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include directory
include_directories(${CMAKE_SOURCE_DIR}/include)
add_subdirectory(tests)

# Add an example executable
add_executable(test_main src/main.cpp)

