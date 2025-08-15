cmake_minimum_required(VERSION 3.9)

project(
    vicar_parser
    VERSION 1.0
    DESCRIPTION "VICAR files parser and processor" 
    LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Debug postfix
set(CMAKE_DEBUG_POSTFIX d)

#add_subdirectory(src)

add_library(vicar_parser STATIC
    src/parser.cpp
    src/vicar.cpp
    src/image/image_builder.cpp
    src/image/tiff_image.cpp
    src/image/pgm_image.cpp
)

add_subdirectory(examples)

set_target_properties(vicar_parser PROPERTIES VERSION ${PROJECT_VERSION})

set(PUBLIC_HEADERS
    include/parser.hpp
    include/vicar.hpp
)

set_target_properties(vicar_parser PROPERTIES
    PUBLIC_HEADER "${PUBLIC_HEADERS}"
)


target_include_directories(vicar_parser PRIVATE .)

target_include_directories(vicar_parser
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/image/include
)

include(GNUInstallDirs)
install(TARGETS vicar_parser
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})