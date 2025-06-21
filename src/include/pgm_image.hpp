#ifndef _PGM_IMAGE_HPP_
#define _PGM_IMAGE_HPP_

#include <cstdint>
#include <string>
#include <fstream>
#include <iostream>
#include <math.h>

constexpr int MAX_UINT8 = 255;

constexpr int CHAR_BITMAP_WIDTH = 8;
constexpr int CHAR_BITMAP_HEIGHT = 16;

class PgmImage {
public:

    PgmImage(const std::string filename, std::size_t width, std::size_t Height, std::size_t max_value);
    ~PgmImage();

    void put_pixel(std::size_t x, std::size_t y, uint16_t value);
    void draw_square(std::size_t x, std::size_t y, uint64_t size, uint16_t value);
    void draw_char(char c, std::size_t font_size, std::size_t x, std::size_t y, uint16_t value);
    void draw_text(const std::string str, std::size_t font_size, std::size_t x, std::size_t y, uint16_t value);

    void set_max_value(int max_value);

private:
    std::ofstream file;
    std::size_t width, height, pixel_size, header_size;

};

#endif // _PGM_IMAGE_HPP_