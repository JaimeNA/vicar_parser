#include <vicar.hpp>

// === Public ===

Vicar::Vicar(const std::string filename) {
    parse(filename);
}

Vicar::~Vicar() {
    // TODO
}

// === Private ===
int Vicar::get_integer(std::string str) {
    size_t i = 0;

    // Travese string until equals or end-of-string
    while (str[i] != '\0' && str[i++] != '=');

    if (str[i] == '\0') {
        // TODO: Error handling
    }

    return std::stoi(str);
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
    file >> token;
    std::cout << get_integer(token) << std::endl;


    // while (file >> token) {
    //     std::cout << token << std::endl;


    // }

}

