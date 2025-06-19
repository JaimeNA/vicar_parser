#include <vicar.hpp>

// === Public ===

Vicar::Vicar(Metadata metadata, BinaryLabel bin_label, Layout layout, Dimensions dimensions) {
    this->metadata = metadata;
    this->bin_label = bin_label;
    this->layout = layout;
    this->dimensions = dimensions;
}


Vicar::~Vicar() {
    //TODO: Manage
}

void Vicar::print() {
    std::cout << "====== DATA ======" << std::endl << std::endl;

    std::cout << "Binary label: " << bin_label.type << std::endl;

    // Layout attributes
    std::cout << "--- LAYOUT --- " << std::endl;
    std::cout << "Label storage area: " << layout.lblsize << std::endl;
    std::cout << "Buffer size(obsolete): " << layout.bufsize << std::endl;
    std::cout << "Dimensions: " << layout.dim << std::endl;
    std::cout << "EOL: " << layout.eol << std::endl;
    std::cout << "Record size: " << layout.recsize << std::endl;
    std::cout << "Number of lines: " << layout.num_lines << std::endl;
    std::cout << "Number of samples: " << layout.num_samples << std::endl;
    std::cout << "Number of bands: " << layout.num_bands << std::endl;

    // Dimensions
    std::cout << "--- DIMENSIONS --- " << std::endl;
    std::cout << "First dimension size: " << dimensions.size_first << std::endl;
    std::cout << "Second dimension size: " << dimensions.size_second << std::endl;
    std::cout << "Third dimension size: " << dimensions.size_third << std::endl;
    std::cout << "Fourth dimension size: " << dimensions.size_fourth << std::endl;

}

