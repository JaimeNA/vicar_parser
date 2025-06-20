#include <parser.hpp>

int main(int argc, char **argv) {

    Parser parser("resources/test1.img");

    Vicar img = parser.parse();

    img.print();
    img.make_pgm("test.pgm");
}
