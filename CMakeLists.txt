cmake_minimum_required(VERSION 3.23)
project(
    vicar
    VERSION 0.0.1
    DESCRIPTION "VICAR files parser and processor" 
    LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Debug postfix
set(CMAKE_DEBUG_POSTFIX d)

add_subdirectory(src)

# === Example ===
add_executable(main)
target_sources(main PRIVATE "examples/main.cpp")
target_link_libraries(main PRIVATE vicar)
