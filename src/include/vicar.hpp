#ifndef _VICAR_HPP_
#define _VICAR_HPP_

#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <unordered_map>

#define DEBUG_LOG(x) "DEBUG::" << x

/**
 * @brief Format of image pixels (e.g., BYTE, HALF, REAL, etc.)
 */
enum class Format{BYTE = 0, HALF, FULL, REAL, DOUB, COMP};
enum class Type{IMAGE = 0, PARMS, PARM, PARAM, GRAPH1, GRAPH2, GRAPH3, TABULAR};
enum class Organization{BSQ = 0, BIL, BIP};
enum class Host{ALLIANT = 0, CRAY, DECSTATN, HP_700, MAC_AUX, MAC_MPW, SGI, SUN_3, SUN_4, TEK, VEX_VMS};
enum class IntFtm{LOW = 0, HIGH};
enum class RealFtm{IEEE = 0, RIEEE, VAX};

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

class Vicar {
public:
	Vicar(Metadata metadata, BinaryLabel bin_label, Layout layout, Dimensions dimensions);
	~Vicar();

    void print();

private:
    // === System labels ===
    Metadata metadata;
    BinaryLabel bin_label;

    Layout layout;
    Dimensions dimensions;
    
    // === Property labels ===
    std::vector<std::string> properties;

    // === History labels ===
    std::vector<std::string> history;
};

#endif // _VICAR_HPP_