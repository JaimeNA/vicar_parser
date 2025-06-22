/*
 * Abstract class, serves as an interface for all the rendering utils for
 * the various graphical files and formats
 */

#ifndef _IMAGE_BUILDER_HPP_
#define _IMAGE_BUILDER_HPP_

#include <cstdint>
#include <string>
#include <fstream>
#include <iostream>
#include <math.h>

constexpr int CHAR_BITMAP_WIDTH = 8;
constexpr int CHAR_BITMAP_HEIGHT = 16;

class ImageBuilder {
    public:

    ImageBuilder(const std::string filename, std::size_t width, std::size_t Height);
    ~ImageBuilder();

    virtual void put_pixel(int x, int y, int value);
    void draw_square(int x, int y, int size, int value);
    void draw_char(char c, int font_size, int x, int y, int value);
    void draw_text(const std::string str, int font_size, int x, int y, int value);

    void set_max_value(int max_value);

protected:
    std::ofstream file;
    std::size_t width, height;

};

#endif // _IMAGE_BUILDER_HPP_