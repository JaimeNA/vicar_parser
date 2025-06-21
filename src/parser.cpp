#include <parser.hpp>

std::unordered_map<std::string, Format> string_to_format = { { "BYTE", Format::BYTE },
                                                    { "HALF", Format::HALF },
                                                    { "FULL", Format::FULL },
                                                    { "REAL", Format::REAL },
                                                    { "DOUB", Format::DOUB },
                                                    { "COMP", Format::COMP },
                                                    { "WORD", Format::HALF },   // Obsolete values and their equivalent
                                                    { "LONG", Format::FULL },
                                                    { "COMPLEX", Format::COMP } };

std::unordered_map<std::string, Type> string_to_type = { { "IMAGE", Type::IMAGE },
                                                    { "PARMS", Type::PARMS },
                                                    { "PARM", Type::PARM },
                                                    { "PARAM", Type::PARAM },
                                                    { "GRAPH1", Type::GRAPH1 },
                                                    { "GRAPH2", Type::GRAPH2 },
                                                    { "GRAPH3", Type::GRAPH3 },   // Obsolete values and their equivalent
                                                    { "TABULAR", Type::TABULAR } };

std::unordered_map<std::string, Organization> string_to_org = { { "BSQ", Organization::BSQ },
                                                    { "BIL", Organization::BIL },
                                                    { "BIP", Organization::BIP } };

std::unordered_map<std::string, Host> string_to_host = { { "ALLIANT", Host::ALLIANT },
                                                    { "CRAY", Host::CRAY },
                                                    { "DECSTATN", Host::DECSTATN },
                                                    { "MAC_AUX", Host::MAC_AUX },
                                                    { "MAC_MPW", Host::MAC_MPW },
                                                    { "SGI", Host::SGI },
                                                    { "SUN_3", Host::SUN_3 },
                                                    { "SUN_4", Host::SUN_4 },
                                                    { "TEK", Host::TEK },
                                                    { "VEX_VMS", Host::VEX_VMS } };

std::unordered_map<std::string, IntFtm> string_to_int_ftm = { { "LOW", IntFtm::LOW },
                                                    { "HIGH", IntFtm::HIGH } };

std::unordered_map<std::string, RealFtm> string_to_real_ftm = { { "IEEE", RealFtm::IEEE },
                                                    { "RIEEE", RealFtm::RIEEE },
                                                    { "VAX", RealFtm::VAX } };

Parser::Parser(const std::string filename) {
    
    std::cout << DEBUG_LOG("Attempting to open: ") << filename << std::endl;

    // Open file for reading in binary mode
    file.open(filename, std::ios::in | std::ios::binary | std::ios::ate);
    
    if (!file.is_open()) {
        std::cerr << "ERROR::Failed to open file: " << filename << std::endl;
        //return null;
    }

    size_t size = 0;

    // Get number of bytes
    file.seekg(0, std::ios::end);
    size = file.tellg();

    std::cout << DEBUG_LOG("File size: ") << size << std::endl;

    // Get labels size, always the first label
    file.seekg(0, std::ios::beg);
    std::string str;
    if(file >> str) {
        int i = str.find('=') + 1;

        lblsize = std::stoi(str.substr(i));
    } else {
        std::cerr << "ERROR::Failed to fetch labels size" << std::endl;
    }

}

Parser::~Parser() {
    if (file.is_open()) file.close();
}

Vicar Parser::parse() {
   
    // NOTE: Any program attempting to read a VICAR file should be able to accept 
    // (and ignore) system label items it doesn't understand

    /*
    * This program looks only for the mandatory labels, support for others
    * may be later implemented.
    */

    // === System labels ===
    std::string token;

    // Metadata
    Metadata meta;

    meta.format = string_to_format[get_string("FORMAT")];
    meta.type = string_to_type[get_string("TYPE")];
    meta.org = string_to_org[get_string("ORG")];
    meta.host = string_to_host[get_string("HOST")];
    meta.int_ftm = string_to_int_ftm[get_string("INTFMT")];
    meta.real_ftm = string_to_real_ftm[get_string("REALFMT")];

    // Binary data format
    BinaryLabel bin_label;

    bin_label.host = string_to_host[get_string("BHOST")];
    bin_label.int_ftm = string_to_int_ftm[get_string("BINTFMT")];
    bin_label.real_ftm = string_to_real_ftm[get_string("BREALFMT")];
    bin_label.type = get_string("BLTYPE");
    bin_label.num_bytes_prefix = get_integer("NBB");
    bin_label.num_lines_header = get_integer("NLB");
    
    // Layout
    Layout layout;

    layout.lblsize = lblsize;
    layout.bufsize = get_integer("BUFSIZ"); // Obsolete
    layout.dim = get_integer("DIM");
    layout.eol = get_integer("EOL");
    layout.recsize = get_integer("RECSIZE");
    layout.num_lines = get_integer("NLB");
    layout.num_samples = get_integer("NS");
    layout.num_bands = get_integer("NB");

    // Dimensions
    Dimensions dim;

    try {
        dim.size_first = get_integer("N1");
    } catch(std::invalid_argument e) {
        // For more data, see doc
        if (meta.org == Organization::BSQ)
            dim.size_first = layout.num_samples;
        else 
            dim.size_first = layout.num_bands;
    }

    try {
        dim.size_second = get_integer("N2");
    } catch(std::invalid_argument e) {
        if (meta.org == Organization::BSQ)
            dim.size_second = layout.num_lines;
        else if (meta.org == Organization::BIL)
            dim.size_second = layout.num_bands;
        else 
            dim.size_second = layout.num_samples;
    }

    try {
        dim.size_third = get_integer("N3");
    } catch(std::invalid_argument e) {
        // For more data, see doc
        if (meta.org == Organization::BSQ)
            dim.size_third = layout.num_bands;
        else 
            dim.size_third = layout.num_lines;
    }

    dim.size_fourth = 0;

    std::unordered_map<std::string, std::vector<std::string>> p = get_properties();

    for (auto i : p) {
            std::cout << i.first << std::endl;
        for (auto j : i.second)
            std::cout << "  " << j << std::endl;
    }

    return Vicar(
        meta,                                           // Metadata
        bin_label,                                      // Binary label string
        layout,                                         // Image layout (dimensions, bands, etc.)
        dim,                                            // Dimension info
        get_image_records(meta, bin_label, layout, dim),// Parsed image data
        get_properties(),                               // Image properties
        get_history()                                   // Image processing history
    );
}

// === Labels area ===

/* Returns occurrences of a char inside a string */
static int get_occurrences(std::string str, char c) {
    int count = 0;

    for (char i : str) {
        if (i == c)
            count++;
    }

    return count;
}

/* Returns next token if there is any, ingores whitespaces inside quoted text */
std::optional<std::string> Parser::get_next_token(int end) {
    std::string to_return;

    if (file >> to_return && file.tellg() < end) {

        // Check for quotes, if the number isnt even, find the whole string
        int quotes_count = get_occurrences(to_return, '\'');
        if (quotes_count % 2) {

            std::string next_token;
            while (file >> next_token && file.tellg() < end && quotes_count % 2) {
                to_return += " " + next_token;  // Add a whitespace in between for better clarity
                quotes_count = get_occurrences(to_return, '\'');
            }

            // Check if the operation was successful
            if (quotes_count % 2)
                return std::nullopt;
        }

    } else {
        return std::nullopt;
    }

    return to_return;

}

/* Returns first position of the token within the file if successful, otherwise returns -1 */
bool Parser::get_token(std::string *str, std::string token) {
    file.seekg( 0, std::ios::beg );
    int eq_pos = 0;

    // Look for a value with the token before the '=', and stop if reached end of labels
    auto result = get_next_token(lblsize);

    while (result) {
        
        eq_pos = result.value().find('=');

        if (eq_pos == std::string::npos)
            continue;

        if (result.value().substr(0, eq_pos) == token){
            *str = result.value();
            return true;
        }

        result = get_next_token(lblsize);
    }

    return false;
}

std::string Parser::get_value(std::string token) {
    size_t i = token.find('=');

    // Check if '=' wasn't found
    if (i == std::string::npos || i + 1 >= token.size()) {
        throw std::invalid_argument("ERROR::GET_VALUE::Not found or invalid");
        // TODO: Error handling, std::optional<T>
    }

    i++;    // Start after the '='
    
    // Use subtring starting with i
    return token.substr(i);
}

std::string Parser::get_string(std::string token) {
    std::string str;

    if (get_token(&str, token) == -1) {
        throw std::invalid_argument("ERROR::STRING::Invalid file");
    }

    std::string aux = get_value(str);

    // Remove first and last characters
    return aux.substr(1, aux.size()-2);
}

int Parser::get_integer(std::string token) {
    std::string str;

    if (get_token(&str, token) == -1) {
        throw std::invalid_argument("ERROR::INTEGER::Invalid file");
    }

    return std::stoi(get_value(str));
}

float Parser::get_real(std::string token) {
    // TODO: Add support for RIEEE and VAX
    std::string str;

    if (get_token(&str, token) == -1) {
        throw std::invalid_argument("ERROR::REAL::Invalid file");
    }

    return std::stof(get_value(str));
}


std::unordered_map<std::string, std::vector<std::string>> Parser::get_history() {
std::unordered_map<std::string, std::vector<std::string>> to_return;

    
    // Store current property here
    std::string current;

    if (!get_token(&current, "TASK")) {
        std::cout << DEBUG_LOG("No history!") << std::endl;

        return to_return;
    }

    to_return[current] = std::vector<std::string>(); // Create new vector

    auto token = get_next_token(lblsize);

    // Traverse file until end of labels or start of history
    while (token) {
        if (token.value().find("PROPERTY") != std::string::npos) {
            current = token.value();

            to_return[current] = std::vector<std::string>();
        } else {
            to_return[current].push_back(token.value());
        }

        token = get_next_token(lblsize);
    }

    return to_return;
}

std::unordered_map<std::string, std::vector<std::string>> Parser::get_properties() {
    std::unordered_map<std::string, std::vector<std::string>> to_return;
    
    // Store current property here
    std::string current;

    if (!get_token(&current, "PROPERTY")) {
        std::cout << DEBUG_LOG("No properties!") << std::endl;

        return to_return;
    }

    /*
     * Now, after running get_token, the current index inside the file points to the
     * first property.
     */

    to_return[current] = std::vector<std::string>(); // Create new vector

    auto token = get_next_token(lblsize);

    // Traverse file until end of labels or start of history
    while (token) {
        if (token.value().find("PROPERTY") != std::string::npos) {
            current = token.value();

            to_return[current] = std::vector<std::string>();
        } else if (token.value().find("TASK") != std::string::npos) { // Check if the history labels section was reached
            break;
        }else {
            to_return[current].push_back(token.value());
        }

        token = get_next_token(lblsize);
    }

    return to_return;
}

// === Image area ===
// TODO: Define internal structs more clearly, check if there is a better way of doing this
std::vector<ImageRecord> Parser::get_image_records(Metadata &meta, BinaryLabel &bin_label, Layout &layout, Dimensions &dim) {
    size_t records_size = dim.size_second*dim.size_third;
    std::vector<ImageRecord> to_return(records_size);

    // Get number of bytes per pixel
    int pixel_size = 1;
    switch (meta.format) {
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

    // Move to the beggining of image records
    size_t binary_header_size = bin_label.num_lines_header*layout.recsize;
    
    file.seekg(layout.lblsize + binary_header_size, std::ios::beg );

    char* p_data; // Used for reading chunks of the file

    for (int i = 0; i < records_size; i++) {
        to_return[i].binary_prefix = std::vector<uint8_t>(bin_label.num_bytes_prefix);  // Initialize it

        p_data = reinterpret_cast<char*>(to_return[i].binary_prefix.data());
        file.read(p_data, bin_label.num_bytes_prefix);  // Read chunk of data


        to_return[i].data = std::vector<uint8_t>(dim.size_first*pixel_size);  // Initialize it

        p_data = reinterpret_cast<char*>(to_return[i].data.data());
        file.read(p_data, dim.size_first*pixel_size);  // Read chunk of data

        if (!file) {
            std::cerr << "ERROR::IMAGE_RECORDS: Reading record " << i << std::endl;
            break;
        }
    }

    return to_return;
}


