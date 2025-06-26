#ifndef _TIFF_IMAGE_HPP_
#define _TIFF_IMAGE_HPP_

#include <cstring>

#include "image_builder.hpp"

constexpr int HEADER_SIZE = 8;

constexpr int DESCRIPTOR_SIZE = 12;

constexpr int DIRECTORY_SIZE = 16;   // Let's start with 1 for simplicity
constexpr int DIRECTORY_OFFSET = HEADER_SIZE + 2; // Two byte offset for directory size
constexpr int DATA_OFFSET = DIRECTORY_OFFSET + DIRECTORY_SIZE*12 + 4; // Four empty bytes after the ifd

constexpr int TIFF_ID = 42;

enum class TiffImageType{BILEVEL = 0, GRAYSCALE, PALETTE_COLOR, RGB};

class TiffImage : public ImageBuilder {
public:

    TiffImage(const std::string filename, std::size_t width, std::size_t height, TiffImageType type);

    void put_pixel(int x, int y, int value);

private:
    TiffImageType type;
    uint32_t next_ifd_offset = 2; // Skipts the elem count
    uint16_t ifd_elem_count = 0;

    void write_header();
    void write_ifd_entry(uint16_t tag, uint16_t type, uint32_t count, uint32_t offset);

};


#endif // _TIFF_IMAGE_HPP_