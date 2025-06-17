#include <vicar.hpp>

// === Public ===

Vicar::Vicar(const std::string filename) {
    parse(filename);
}

Vicar::~Vicar() {
    // TODO
}

// === Private ===
int getInteger(std::string str) {
    while ()
}

void Vicar::parse(const std::string filename) {
    std::ifstream file;

    std::cout << DEBUG_LOG("Attempting to open: ") << filename << std::endl;

    // Open file for reading in binary mode
    file.open(filename, std::ios::in | std::ios::binary | std::ios::ate);
    
    if (!file.is_open()) {
        std::cerr << "ERROR::Failed to open file: " << filename << std::endl;
        return;
    }

    size_t size = 0;

    // Get number of bytes
    file.seekg( 0, std::ios::end );
    size = file.tellg();

    // Go to the beggining of the file
    file.seekg( 0, std::ios::beg );

    std::cout << DEBUG_LOG("File size: ") << size << std::endl;

    std::string token;
    
    // === System labels ===

    // LBLSIZE



    while (file >> token) {
        std::cout << token << std::endl;


    }

}

