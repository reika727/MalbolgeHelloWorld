#include "malbolge_machine.hpp"
#include <iostream>
#include <fstream>

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cerr << "invalid command line" << std::endl;
        return EXIT_FAILURE;
    }
    std::ifstream ifs{argv[1]};
    if (!ifs) {
        std::cerr << "can't open file" << std::endl;
        return EXIT_FAILURE;
    }
    malbolge_machine mm(ifs);
    while (!mm.exec_one_step(std::cin, std::cout)) {
        ;
    }
    return EXIT_SUCCESS;
}
