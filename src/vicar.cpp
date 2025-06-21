#include <vicar.hpp>

//TODO: Switch to PNG or TIFF
// === Public ===

Vicar::Vicar(Metadata metadata, BinaryLabel bin_label, Layout layout, Dimensions dimensions, std::vector<ImageRecord> image_records) {
    this->metadata = metadata;
    this->bin_label = bin_label;
    this->layout = layout;
    this->dimensions = dimensions;

    this->image_records = image_records;
}


Vicar::~Vicar() {
    //TODO: Manage
}

void Vicar::print() {
    std::cout << "====== DATA ======" << std::endl << std::endl;

    // Binary label
    std::cout << "--- BINARY LABEL --- " << std::endl;
    std::cout << "Binary label: " << bin_label.type << std::endl;
    std::cout << "Bytes of binary prefix: " << bin_label.num_bytes_prefix << std::endl;
    std::cout << "Lines of binary header: " << bin_label.num_lines_header << std::endl << std::endl;

    // Layout attributes
    std::cout << "--- LAYOUT --- " << std::endl;
    std::cout << "Label storage area: " << layout.lblsize << std::endl;
    std::cout << "Buffer size(obsolete): " << layout.bufsize << std::endl;
    std::cout << "Dimensions: " << layout.dim << std::endl;
    std::cout << "EOL: " << layout.eol << std::endl;
    std::cout << "Record size: " << layout.recsize << std::endl;
    std::cout << "Number of lines: " << layout.num_lines << std::endl;
    std::cout << "Number of samples: " << layout.num_samples << std::endl;
    std::cout << "Number of bands: " << layout.num_bands << std::endl << std::endl;

    // Dimensions
    std::cout << "--- DIMENSIONS --- " << std::endl;
    std::cout << "First dimension size: " << dimensions.size_first << std::endl;
    std::cout << "Second dimension size: " << dimensions.size_second << std::endl;
    std::cout << "Third dimension size: " << dimensions.size_third << std::endl;
    std::cout << "Fourth dimension size: " << dimensions.size_fourth << std::endl << std::endl;

}

void Vicar::make_pgm(const std::string filename) {

    if (metadata.type != Type::IMAGE) {
        std::cerr << "ERROR::DRAW_SQUARE: Not an image file" << std::endl;
        return ;
    }
    
    int pixel_size = get_pixel_size();
   
    if (pixel_size > 2) {
        std::cerr << "ERROR::MAKE_PGM: Unsopported pixel size" << std::endl;
    }

    std::cout << DEBUG_LOG("Pixel size: ") << pixel_size << std::endl;

    int min_value = get_min_value();

    PgmImage image(filename, dimensions.size_first, dimensions.size_second, get_max_value() - min_value);

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

            image.put_pixel(j, i, *((uint16_t*)value) - min_value);
        }
    }

 //  draw_data(image, 8);

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

// Inefficient, remove
int Vicar::get_max_value() {

    int max_value = 0;  // For better displaying

    int pixel_size = get_pixel_size();
    uint8_t value[MAX_PIXEL_SIZE]; // Array used for storing the value

    for (int i = 0; i < dimensions.size_second; i++) {
        for (int j = 0; j < dimensions.size_first; j++) {

            // Create the pixel value
            if (pixel_size == 1) {
                value[0] = image_records[i].data[j];
            } else {
                // Create the pixel value
                for (int k = 0; k < pixel_size; k++) {
                    value[k] = image_records[i].data[j*2 + k];
                }
            }

           // std::cout << DEBUG_LOG("Value: ") << *((uint16_t*)value) << std::endl;

            if (max_value <  *((uint16_t*)value))
                max_value =  *((uint16_t*)value);
        }
    }
    return max_value;
}

// Inefficient, remove
int Vicar::get_min_value() {

    int min_value = -1;  // For better displaying

    int pixel_size = get_pixel_size();
    uint8_t value[MAX_PIXEL_SIZE]; // Array used for storing the value

    for (int i = 0; i < dimensions.size_second; i++) {
        for (int j = 0; j < dimensions.size_first; j++) {

            // Create the pixel value
            if (pixel_size == 1) {
                value[0] = image_records[i].data[j];
            } else {
                // Create the pixel value
                for (int k = 0; k < pixel_size; k++) {
                    value[k] = image_records[i].data[j*2 + k];
                }
            }

           // std::cout << DEBUG_LOG("Value: ") << *((uint16_t*)value) << std::endl;

            if (min_value == -1 || min_value >  *((uint16_t*)value))
                min_value =  *((uint16_t*)value);
        }
    }
    return min_value;
}

void Vicar::draw_data(PgmImage &image, int font_size) {

    int max_value = get_max_value();
    int current_row =1; // Start at text row 5

    // Binary label
    image.draw_text("--- BINARY LABEL --- ", font_size, 100, font_size * 2 * current_row++, max_value);

    image.draw_text("Bytes of binary prefix: " + std::to_string(bin_label.num_bytes_prefix), font_size, 100, font_size * 2 * current_row++, max_value);
    image.draw_text("Lines of binary header: " + std::to_string(bin_label.num_lines_header), font_size, 100, font_size * 2 * current_row++, max_value);

    // Layout attributes
    image.draw_text("--- LAYOUT --- ", font_size, 100, font_size * 2 * current_row++, max_value);

    image.draw_text("Label storage area: "  + std::to_string(layout.lblsize), font_size, 100, font_size * 2 * current_row++, max_value);
    image.draw_text("Buffer size(obsolete): " + std::to_string(layout.bufsize), font_size, 100, font_size * 2 * current_row++, max_value);
    image.draw_text("Dimensions: " + std::to_string(layout.dim), font_size, 100, font_size * 2 * current_row++, max_value);
    image.draw_text("EOL: " + std::to_string(layout.eol), font_size, 100, font_size * 2 * current_row++, max_value);
    image.draw_text("Record size: " + std::to_string(layout.recsize), font_size, 100, font_size * 2 * current_row++, max_value);
    image.draw_text("Number of lines: " + std::to_string(layout.num_lines), font_size, 100, font_size * 2 * current_row++, max_value);
    image.draw_text("Number of samples: " + std::to_string(layout.num_samples), font_size, 100, font_size * 2 * current_row++, max_value);
    image.draw_text("Number of bands: " + std::to_string(layout.num_bands), font_size, 100, font_size * 2 * current_row++, max_value);
}