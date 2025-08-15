#include "pgm_image.hpp"

PgmImage::PgmImage(const std::string filename, std::size_t width, std::size_t height, int pixel_size)
        : ImageBuilder(filename, width, height, pixel_size) {
  
    // NOTE: Max pixel size for PGM images is 2
    if (pixel_size > 2)
        throw std::invalid_argument("ERROR::PGM_IMAGE::Invalid pixel size");

    // https://netpbm.sourceforge.net/doc/pgm.html
    // Use a string the buffer to place the parsed metadata before writing it into the file
    std::string buffer = "P5";

    file.write(&buffer[0], buffer.size()); // Add identifier
    file.put('\n');

    // Width
    buffer = std::to_string(width);

    file.write(&buffer[0], buffer.size());
    file.put('\n'); // Add whitespace, in this case, a new line

    // Height
    buffer = std::to_string(height);

    file.write(&buffer[0], buffer.size());
    file.put('\n');

    buffer = std::to_string(pow(2, pixel_size*8)); // Get the max absolute possible value, based on the pixel size
    
    file.write(&buffer[0], buffer.size());
    file.put('\n');

    header_size = file.tellp() + 1; // Get the header size by checking the current position
}

void PgmImage::put_pixel(int x, int y, int value) {
    
    if (x < 0 || y < 0 || x >= width || y >= height)
        throw std::invalid_argument("ERROR::PGM::Out of bounds");

    std::size_t offset = (width*y*pixel_size) + x*pixel_size;
    file.seekp(header_size + offset);

    file.write(reinterpret_cast<char*>(&value), pixel_size);
}