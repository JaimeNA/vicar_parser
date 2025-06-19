#include <vicar.hpp>

// === Public ===

Vicar::Vicar(const std::string filename) {
    parse(filename);
}

Vicar::~Vicar() {
    // TODO
}

// === Private ===

/* Returns string length if successful, otherwise returns -1 */
bool Vicar::get_token(std::ifstream &file, std::string *str, std::string token) {
    if (!file.is_open()) {
        throw std::invalid_argument("ERROR::GET_TOKEN::Invalid file");
    }

    file.seekg( 0, std::ios::beg );
    int eq_pos = 0;

    while (file >> *str) {
        eq_pos = str->find('=');

        if (eq_pos == std::string::npos)
            continue;

        if (str->substr(0, eq_pos) == token)
            return true;
    }

    return false;
}

std::string Vicar::get_value(std::string token) {
    size_t i = token.find('=');
    i++;    // Start after the '='

    // Check if '=' wasn't found
    if (token[i] == '\0') {
        throw std::invalid_argument("ERROR::GET_VALUE::Invalid argument");
        // TODO: Error handling, std::optional<T>
    }
    
    // Use subtring starting with i
    return token.substr(i);
}

std::string Vicar::get_string(std::string token) {
    std::string aux = get_value(token);

    // Remove first and last characters
    return aux.substr(1, aux.size()-2);
}

int Vicar::get_integer(std::string token) {
    return std::stoi(get_value(token));
}

float Vicar::get_real(std::string token) {
    // TODO: Add support for RIEEE and VAX
    return std::stof(get_value(token));
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

    std::cout << DEBUG_LOG("File size: ") << size << std::endl;

    // === System labels ===
    // NOTE: Any program attempting to read a VICAR file should be able to accept 
    // (and ignore) system label items it doesn't understand

    /*
    * This program looks only for the mandatory labels, support for others
    * may be later implemented.
    */

    std::string token;

    // LBLSIZE; Look for it and parse
    if (!get_token(file, &token, "LBLSIZE")) {
        throw std::invalid_argument("ERROR::PARSE::Invalid file");
    }

    lblsize = get_integer(token);
    std::cout << "LBLSIZE: " << lblsize << std::endl;

    // Format
    if (!get_token(file, &token, "FORMAT")) {
        throw std::invalid_argument("ERROR::PARSE::Invalid file");
    }

    format = stringToFormat[get_string(token)];
    std::cout << "FORMAT: " << get_string(token) << std::endl;


    // while (file >> token) {
    //     std::cout << token << std::endl;


    // }

}

