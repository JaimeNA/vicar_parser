cmake_minimum_required(VERSION 3.9)

# === Example ===
add_executable(main)
target_sources(main PRIVATE "main.cpp")
target_link_libraries(main PRIVATE vicar_parser)

target_include_directories(main PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)