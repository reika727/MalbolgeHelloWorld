#include "malbolge_machine.hpp"
#include <iostream>
#include <fstream>
#include <stdexcept>

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cerr << "invalid command line" << std::endl;
        return EXIT_FAILURE;
    } else if (std::ifstream ifs{argv[1]}) {
        malbolge_machine mm(ifs);
        while (!mm.exec_one_step(std::cin, std::cout)) {
            ;
        }
        return EXIT_SUCCESS;
    } else {
        std::cerr << "can't open file" << std::endl;
        return EXIT_FAILURE;
    }
}
