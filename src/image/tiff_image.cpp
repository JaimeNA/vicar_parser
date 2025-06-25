#include "tiff_image.hpp"

TiffImage::TiffImage(const std::string filename, std::size_t width, std::size_t height, TiffImageType type)
        : ImageBuilder(filename, width, height) {
 
    this->type = type;

    write_header();
    
    write_ifd_entry(256, 3, 1, 100);               // ImageWidth
    write_ifd_entry(257, 3, 1, 100);              // ImageLength
    write_ifd_entry(258, 3, 1, 8);                   // BitsPerSample
    write_ifd_entry(259, 3, 1, 1);                   // No compression
    write_ifd_entry(262, 3, 1, 1);                   // BlackIsZero
    write_ifd_entry(273, 4, 1, DIRECTORY_OFFSET+DIRECTORY_SIZE + 200);        // Offset to image data
    write_ifd_entry(277, 3, 1, 1);                   // SamplesPerPixel
    write_ifd_entry(278, 4, 1, 1);              // RowsPerStrip
    write_ifd_entry(279, 4, 1, 100*100);    // Byte count of image data

    file.seekp(DIRECTORY_OFFSET + next_ifd_offset);

    uint32_t to_write = 0;
    file.write(reinterpret_cast<char*>(&to_write), 4);

    file.seekp(DIRECTORY_OFFSET+DIRECTORY_SIZE + 200);
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            int value = i % 2;
            file.write(reinterpret_cast<char*>(&value), 1);
        }
    }
}

void TiffImage::put_pixel(int x, int y, int value) {
    
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

    descriptor[0] = tag & 0xFF;
    descriptor[1] = (tag >> 8) & 0xFF;

    descriptor[2] = type & 0xFF;    // TODO: Use memcpy too?
    descriptor[3] = (type >> 8) & 0xFF;

    std::memcpy(&descriptor[4], &count, 4); 

    // TODO: Remove magic number
    std::memcpy(&descriptor[8], &offset, 4); 

    // Write into file
    file.write(descriptor, DESCRIPTOR_SIZE);

    // Update the offset of the next descriptor
    next_ifd_offset += DESCRIPTOR_SIZE;

    // Update the IFD descriptors count
    ifd_elem_count++;

    file.seekp(DIRECTORY_OFFSET);
    file.write(reinterpret_cast<char*>(&ifd_elem_count), 4);
}