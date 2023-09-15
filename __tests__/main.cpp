#include "malbolge_machine.hpp"
#include <iostream>
#include <fstream>
#include <stdexcept>

int main(int argc, char *argv[])
{
    if (argc != 2) {
        throw std::runtime_error("invalid command line");
    } else if (std::ifstream ifs{argv[1]}) {
        malbolge_machine mm(ifs);
        while (!mm.exec_one_step(std::cin, std::cout)) {
            ;
        }
        return EXIT_SUCCESS;
    } else {
        throw std::runtime_error("can't open file");
    }
}
