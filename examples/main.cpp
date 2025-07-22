#include "parser.hpp"

void print_usage() {
    std::cout << "Usage: ./vicar [Options] [input filename] [output filename]" << std::endl;
    std::cout << "Options: " << std::endl;
    std::cout << "  -d: Draw image metadata" << std::endl;
}

int main(int argc, char **argv) {

    if (argc < 3) {
        print_usage();
        return -1;
    }

    int i = 1;

    bool draw_data = false;

    // Check for arguments
    while (argv[i][0] == '-') {

        // TODO: Add support for more arguments
        switch(argv[i][1]) {
            case 'd':
                draw_data = true;
                break;
            default:
                break;
        };

        i++;
    }

    std::string in_file(argv[i++]);

    Parser parser(in_file);

    Vicar img = parser.parse();

    std::string out_file(argv[i]);
   // img.print();
    img.make_image(out_file, draw_data);

    std::cout << in_file << " " << out_file << std::endl;
}
