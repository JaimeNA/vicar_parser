#ifndef _VICAR_HPP_
#define _VICAR_HPP_

#include <string>
#include <vector>

constexpr int MAX_FORMAT_SIZE = 6; // 6 to fit obsolete value 'COMPLEX' from older images
constexpr int MAX_TYPE_SIZE = 7;
constexpr int MAX_ORG_SIZE = 3;
constexpr int MAX_HOST_SIZE = 8;
constexpr int MAX_INTFMT_SIZE = 3;
constexpr int MAX_REALFTM_SIZE = 5;

/**
 * @brief Format of image pixels (e.g., BYTE, HALF, REAL, etc.)
 */
enum class Format{BYTE = 0, HALF, FULL, REAL, DOUB, COMP};
enum class Type{IMAGE = 0, PARMS, PARM, PARAM, GRAPH1, GRAPH2, GRAPH3, TABULAR};
enum class Organization{BSQ = 0, BIL, BIP};
enum class Host{ALLIANT = 0, CRAY, DECSTATN, HP_700, MAC_AUX, MAC_MPW, SGI, SUN_3, SUN_4, TEK, VEX_VMS};
enum class IntPixels{LOW = 0, HIGH};
enum class RealFtm{IEEE = 0, RIEEE, VAX};

class Vicar {
private:
    // === System labels ===

    // Image format
    Format format;
    Type type;
    Organization org;
    Host host;

    // Binary data format
    IntPixels integer_pixels;
    RealFtm real_ftm;
    Host binary_host;
    IntPixels binary_int_ftm;
    RealFtm binary_real_ftm;
    std::string binary_label_type;

    // Dimensions
    int lblsize;
    int bufsize;
    int dim;
    int eol;
    int recsize;
    int num_lines;
    int num_samples;
    int num_bands;
    int sec_dim_size;
    int third_dim_size;
    int four_dim_size; // Not implemented, defaults to 0
    int num_bytes_binary_prefix;
    int num_lines_binary_header;

    // === Property labels ===
    std::vector<std::string> properties;

    // === History labels ===
    std::vector<std::string> history;

public:
	Vicar(std::string filename);
	~Vicar();
};

#endif // _VICAR_HPP_