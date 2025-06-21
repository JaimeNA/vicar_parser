#include <parser.hpp>

int main(int argc, char **argv) {

    Parser parser("resources/raw/voyager.img");

    Vicar img = parser.parse();

    img.print();
    img.make_pgm("resources/pgm/test.pgm");
}
