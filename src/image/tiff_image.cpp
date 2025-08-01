#include "tiff_image.hpp"

TiffImage::TiffImage(const std::string filename, std::size_t width, std::size_t height, int pixel_size, TiffImageType type)
        : ImageBuilder(filename, width, height, pixel_size) {
 
    this->type = type;

    write_header();
    
    // TODO: Finish defining the TIFF format used for images

    // Write entries

    // Common entries
    write_ifd_entry(256, 3, 1, width);        // ImageWidth
    write_ifd_entry(257, 3, 1, height);       // ImageLength
    write_ifd_entry(258, 3, 1, 8);            // BitsPerSample
    write_ifd_entry(259, 3, 1, 1);            // No compression
    write_ifd_entry(273, 4, 1, DATA_OFFSET);  // Offset to image data
    write_ifd_entry(278, 4, 1, width);        // RowsPerStrip
    write_ifd_entry(279, 4, 1, width*height); // Byte count of image data

    // TODO: Remove magic numbers
    switch(type) {
        case TiffImageType::BILEVEL: 
            throw std::invalid_argument("ERROR::TIFF_IMAGE::Not implemented");
            break;
        case TiffImageType::GRAYSCALE: 
            write_ifd_entry(277, 3, 1, 1); // SamplesPerPixel
            write_ifd_entry(262, 3, 1, 1); // BlackIsZero

            break;
        case TiffImageType::PALETTE_COLOR: 
            throw std::invalid_argument("ERROR::TIFF_IMAGE::Not implemented");
            break;
        case TiffImageType::RGB: 
            write_ifd_entry(262, 3, 1, 2); // Photometric interpretation
            write_ifd_entry(277, 3, 1, 3); // SamplesPerPixel

            write_ifd_entry(282, 4, 1, 1); // XResolution     
            write_ifd_entry(283, 4, 1, 1); // YResolution
            write_ifd_entry(296, 3, 1, 1); // ResolutionUnit
            break;
    };

    file.seekp(DIRECTORY_OFFSET + next_ifd_offset);

    // End descriptor table with 4 null bytes
    uint32_t to_write = 0;
    file.write(reinterpret_cast<char*>(&to_write), 4);

}

void TiffImage::put_pixel(int x, int y, int value) {
    std::size_t offset;

    switch(type) {
        case TiffImageType::BILEVEL: 
            throw std::invalid_argument("ERROR::TIFF_IMAGE::Not implemented");
            break;
        case TiffImageType::GRAYSCALE: 
            offset = (width*y*pixel_size) + x*pixel_size;
            file.seekp(DATA_OFFSET + offset);
            
            file.write(reinterpret_cast<char*>(&value), pixel_size);
            break;
        case TiffImageType::PALETTE_COLOR: 
            throw std::invalid_argument("ERROR::TIFF_IMAGE::Not implemented");
            break;
        case TiffImageType::RGB: 
            offset = (width*y*3) + x*3;
            file.seekp(DATA_OFFSET + offset);
            
            file.write(reinterpret_cast<char*>(&value), 3);
            break;
    };
}

// === Image writing utils ===

void TiffImage::write_header() {
    // https://docs.fileformat.com/image/tiff/

    file.seekp(std::ios::beg);

    // Write header, bytes 0-1
    char header[HEADER_SIZE];

    // Use II byte order(little-endian), bytes 2-3
    header[0] = 0x49;
    header[1] = 0x49;

    // Identification number (42), bytes 4-7
    header[2] = TIFF_ID;
    header[3] = 0x00;   // Leave at 0 since its little-endian

    // Offset of the first IFD(Image File Directory) 
    uint32_t ifd_offset = DIRECTORY_OFFSET;
    std::memcpy(&header[4], &ifd_offset, 4); 

    // Write the header into the file
    file.write(header, HEADER_SIZE);

}

/* Adds an entry to the file directory */
void TiffImage::write_ifd_entry(uint16_t tag, uint16_t type, uint32_t count, uint32_t offset) {

    // Go to the last index
    file.seekp(DIRECTORY_OFFSET + next_ifd_offset);

    // NOTE: Write to descriptor in little-endian format
    char descriptor[DESCRIPTOR_SIZE];

    std::memcpy(&descriptor[0], &tag, 2); 

    std::memcpy(&descriptor[2], &type, 2); 

    std::memcpy(&descriptor[4], &count, 4); 

    std::memcpy(&descriptor[8], &offset, 4); 

    // Write into file
    file.write(descriptor, DESCRIPTOR_SIZE);

    // Update the offset of the next descriptor
    next_ifd_offset += DESCRIPTOR_SIZE;

    // Update the IFD descriptors count
    ifd_elem_count++;

    file.seekp(DIRECTORY_OFFSET);
    file.write(reinterpret_cast<char*>(&ifd_elem_count), 2);
}