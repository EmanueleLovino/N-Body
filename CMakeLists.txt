cmake_minimum_required(VERSION 3.22)
project(nbody LANGUAGES CXX)

# Use C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Generate compile_commands.json for LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---- Fetch Catch2 (needed by tests) ----
include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.2
)

FetchContent_MakeAvailable(Catch2)

# Make Catch's CMake helpers visible
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)

# ---- Add subdirectories ----
add_subdirectory(tests)

# ---- Example executable ----
add_executable(test_main src/main.cpp)

target_include_directories(test_main PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
