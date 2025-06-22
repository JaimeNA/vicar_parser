file(GLOB SRC_FILES CONFIGURE_DEPENDS *.cpp
    image/*.cpp)

# Combine all object files into one static library
add_library(vicar STATIC ${SRC_FILES})

# Include the public interface
target_include_directories(vicar
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include/vicar/>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/image/include/
)

# Add subdirectories for image
add_subdirectory(image)
