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
    std::ofstream out_file(filename, std::ios::out | std::ios::binary);

    if (!out_file) {
        std::cerr << "ERROR::MAKE_PGM: Error opening file!" << std::endl;
        return ;
    }

    // https://netpbm.sourceforge.net/doc/pgm.html
    // Use a string as a buffer to place the parsed metadata before writing it into the file
    std::string buffer = "P5";

    char newline = '\n';

    out_file.write(&buffer[0], buffer.size()); // Add identifier
    out_file.write(&newline, sizeof(newline));

    // Width
    buffer = std::to_string(dimensions.size_first);
    out_file.write(&buffer[0], buffer.size()); 

    out_file.write(&newline, sizeof(newline));  // Add whitespace, in this case, a new line
    // Height
    buffer = std::to_string(dimensions.size_second);
    out_file.write(&buffer[0], buffer.size()); 
    
    out_file.write(&newline, sizeof(newline));

    // TEST
    int pixel_size = 0;
    switch (metadata.format) {
        case Format::BYTE:
            pixel_size = 1;
            break;
        case Format::HALF:
            pixel_size = 2;
            break;
        case Format::FULL:
        case Format::REAL:
            pixel_size = 4;
            break;
        case Format::DOUB:
        case Format::COMP:   // Two reals
            pixel_size = 8;
            break;
    };

    if (pixel_size > 2) {
        std::cerr << "ERROR::MAKE_PGM: Unsopported pixel size" << std::endl;
    }

    buffer = std::to_string((int)pow(2, pixel_size*8) - 1); // Get max value(considering it unsigned)
    //buffer = std::to_string(255);
    out_file.write(&buffer[0], buffer.size()); 
    out_file.write(&newline, sizeof(newline));

    for (int i = 0; i < dimensions.size_second; i++) {
        for (int j = 0; j < dimensions.size_first; j++) {
            int value = image_records[i].data[j];
            //std::cout << DEBUG_LOG("Value: ") << value << std::endl;
            out_file.write(reinterpret_cast<char*>(&value), pixel_size);
        }
    }

    //out_file.write(reinterpret_cast<char*>(&PGM_IDENTIFIER), sizeof(PGM_IDENTIFIER));
    out_file.close();
}