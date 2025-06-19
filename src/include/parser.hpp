#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <unordered_map>

#include <vicar.hpp>

constexpr int MAX_FORMAT_SIZE = 6; // 6 to fit obsolete value 'COMPLEX' from older images
constexpr int MAX_TYPE_SIZE = 7;
constexpr int MAX_ORG_SIZE = 3;
constexpr int MAX_HOST_SIZE = 8;
constexpr int MAX_INTFMT_SIZE = 3;
constexpr int MAX_REALFTM_SIZE = 5;

class Parser {
public:
    Parser(std::string filename);
    ~Parser();

    Vicar parse();

private:
    std::ifstream file;

    // === Helper functions ===

    /* Returns string after '=' */
    bool get_token(std::string *str, std::string token);
    std::string get_value(std::string token);

    std::string get_string(std::string token); // Parse the vicar string, remove ' ' 
    int get_integer(std::string token);
    float get_real(std::string token);
};

#endif // _PARSER_HPP_