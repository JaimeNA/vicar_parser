#ifndef _PGM_IMAGE_HPP_
#define _PGM_IMAGE_HPP_

#include "image_builder.hpp"

constexpr int MAX_UINT8 = 255;

class PgmImage : public ImageBuilder {
public:

    PgmImage(const std::string filename, std::size_t width, std::size_t Height, std::size_t max_value);

    void put_pixel(int x, int y, int value);
    void set_max_value(int max_value);

private:
    std::size_t header_size, pixel_size;

};

#endif // _PGM_IMAGE_HPP_