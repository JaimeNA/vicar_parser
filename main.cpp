#include <parser.hpp>

void print_usage() {
    std::cout << "Usage: ./vicar [input filename] [output filename]" << std::endl;
}

int main(int argc, char **argv) {

    if (argc < 3) {
        print_usage();
        return -1;
    }
    std::string in_file(argv[1]);

    Parser parser(in_file);

    Vicar img = parser.parse();

    std::string out_file(argv[2]);
   // img.print();
    img.make_pgm(out_file);

    std::cout << in_file << " " << out_file << std::endl;
}
