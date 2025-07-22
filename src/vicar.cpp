#include "vicar.hpp"

#include "pgm_image.hpp"
#include "tiff_image.hpp"

// === Public ===

Vicar::Vicar(Metadata metadata,
        BinaryLabel bin_label,
        Layout layout,
        Dimensions dimensions,
        std::vector<ImageRecord> image_records,
        std::unordered_map<std::string, std::vector<std::string>> properties,
        std::unordered_map<std::string, std::vector<std::string>> history) {

    this->metadata = metadata;
    this->bin_label = bin_label;
    this->layout = layout;
    this->dimensions = dimensions;

    this->image_records = image_records;
    this->properties = properties;
    this->history = history;
}


Vicar::~Vicar() {
    //TODO: Manage
}

std::string Vicar::to_string() {

    std::string to_return;

    to_return += "====== DATA ====== \n\n";

    // Binary label
    to_return += "--- BINARY LABEL --- \n";
    to_return += "Binary label: " + bin_label.type + "\n";
    to_return += "Bytes of binary prefix: " + std::to_string(bin_label.num_bytes_prefix) + "\n";
    to_return += "Lines of binary header: " + std::to_string(bin_label.num_lines_header) + "\n\n";

    // Layout attributes
    to_return += "--- LAYOUT --- \n";
    to_return += "Label storage area: " + std::to_string(layout.lblsize) + "\n";
    to_return += "Buffer size(obsolete): " + std::to_string(layout.bufsize) + "\n";
    to_return += "Dimensions: " + std::to_string(layout.dim) + "\n";
    to_return += "EOL: " + std::to_string(layout.eol) + "\n";
    to_return += "Record size: " + std::to_string(layout.recsize) + "\n";
    to_return += "Number of lines: " + std::to_string(layout.num_lines) + "\n";
    to_return += "Number of samples: " + std::to_string(layout.num_samples) + "\n";
    to_return += "Number of bands: " + std::to_string(layout.num_bands) + "\n\n";

    // Dimensions
    to_return += "--- DIMENSIONS --- \n";;
    to_return += "First dimension size: " + std::to_string(dimensions.size_first) + "\n";
    to_return += "Second dimension size: " + std::to_string(dimensions.size_second) + "\n";
    to_return += "Third dimension size: " + std::to_string(dimensions.size_third) + "\n";
    to_return += "Fourth dimension size: " + std::to_string(dimensions.size_fourth) + "\n\n";

    return to_return;
}

void Vicar::make_image(const std::string filename, bool draw_data) {

    if (metadata.type != Type::IMAGE) {
        std::cerr << "ERROR::MAKE_IMAGE: Not an image file" << std::endl;
        return ;
    }
    
    int pixel_size = get_pixel_size();
   
    if (pixel_size > 2) {
        std::cerr << "ERROR::MAKE_IMAGE: Unsopported pixel size" << std::endl;
    }


    TiffImage image(filename, dimensions.size_first, dimensions.size_second, pixel_size, TiffImageType::GRAYSCALE);

    uint8_t value[MAX_PIXEL_SIZE]; // Array used for storing the value

    for (int i = 0; i < dimensions.size_second; i++) {
        for (int j = 0; j < dimensions.size_first; j++) {

            if (pixel_size == 1) {
                value[0] = image_records[i].data[j];
            } else {
                // Create the pixel value
                for (int k = 0; k < pixel_size; k++) {
                    value[k] = image_records[i].data[j*2 + k];
                }
            }
            image.put_pixel(j, i, *((unsigned int*)value));
        }
    }

    if (draw_data)
        image.draw_text(to_string(), 8, 100, 100, 0xFF);

}

std::size_t Vicar::get_pixel_size() {
    switch (metadata.format) {
        case Format::BYTE:
            return 1;
            break;
        case Format::HALF:
            return 2;
            break;
        case Format::FULL:
        case Format::REAL:
            return 4;
            break;
        case Format::DOUB:
        case Format::COMP:   // Two reals
            return 8;
            break;
    };
}

