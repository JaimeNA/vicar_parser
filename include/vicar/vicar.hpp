#ifndef _VICAR_HPP_
#define _VICAR_HPP_

#include <cstdint>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <unordered_map>

#define DEBUG_LOG(x) "DEBUG::" << x

constexpr int MAX_PIXEL_SIZE = 8;

/**
 * @brief Format of image pixels (e.g., BYTE, HALF, REAL, etc.)
 */
enum class Format{BYTE = 0, HALF, FULL, REAL, DOUB, COMP};
enum class Type{IMAGE = 0, PARMS, PARM, PARAM, GRAPH1, GRAPH2, GRAPH3, TABULAR};
enum class Organization{BSQ = 0, BIL, BIP};
enum class Host{ALLIANT = 0, CRAY, DECSTATN, HP_700, MAC_AUX, MAC_MPW, SGI, SUN_3, SUN_4, TEK, VEX_VMS};
enum class IntFtm{LOW = 0, HIGH};
enum class RealFtm{IEEE = 0, RIEEE, VAX};

// === Labels area ===
struct Metadata {
    Format format;
    Type type;
    Organization org;
    Host host;
    IntFtm int_ftm;
    RealFtm real_ftm;
};

struct Layout {
    int lblsize;
    int bufsize;
    int dim;
    int eol;
    int recsize;
    int num_lines;
    int num_samples;
    int num_bands;
};

struct BinaryLabel {
    Host host;
    IntFtm int_ftm;
    RealFtm real_ftm;
    std::string type;
    int num_bytes_prefix;
    int num_lines_header;
};

struct Dimensions {
    int size_first;
    int size_second;
    int size_third;
    int size_fourth; // Not implemented, defaults to 0
};

// === Image area ===
struct ImageRecord {
    std::vector<uint8_t> binary_prefix;
    std::vector<uint8_t> data;
};

class Vicar {
public:
	Vicar(Metadata metadata,
        BinaryLabel bin_label,
        Layout layout,
        Dimensions dimensions,
        std::vector<ImageRecord> image_records,
        std::unordered_map<std::string, std::vector<std::string>> properties,
        std::unordered_map<std::string, std::vector<std::string>> history);
	~Vicar();

    void print();
    void make_pgm(const std::string filename);

private:
    size_t get_pixel_size();    // TODO: Move image utils to their own file
    int get_max_value();
    int get_min_value();

    // === Labels area ===
    // System labels
    Metadata metadata;
    BinaryLabel bin_label;

    Layout layout;
    Dimensions dimensions;
    
    // Property labels
    std::unordered_map<std::string, std::vector<std::string>> properties;

    // History labels
    std::unordered_map<std::string, std::vector<std::string>> history;

    // == Image area ===
    std::vector<ImageRecord> image_records;
};

#endif // _VICAR_HPP_