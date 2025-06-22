#include "pgm_image.hpp"

PgmImage::PgmImage(const std::string filename, std::size_t width, std::size_t height, std::size_t max_value)
        : ImageBuilder(filename, width, height) {
  
    // Determine number of bytes for each pixel, max is 2 bytes
    if (max_value <= MAX_UINT8)
        this->pixel_size = 1;
    else
        this->pixel_size = 2;

    // https://netpbm.sourceforge.net/doc/pgm.html
    // Use a string as a buffer to place the parsed metadata before writing it into the file
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

    buffer = std::to_string(max_value);
    
    file.write(&buffer[0], buffer.size());
    file.put('\n');

    header_size = file.tellp() + 1; // Get header size by checking our current position
}

void PgmImage::put_pixel(int x, int y, int value) {
    // TODO: Add bound checks
    // Go to the corresponding row starting

    std::size_t offset = (width*y*pixel_size) + x*pixel_size;
    file.seekp(header_size + offset);

    file.write(reinterpret_cast<char*>(&value), pixel_size);
}