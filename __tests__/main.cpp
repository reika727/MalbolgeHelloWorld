#include "malbolge_machine.hpp"
#include <iostream>
#include <fstream>
#include <stdexcept>

int main(int argc, char *argv[])
{
    try {
        if (argc != 2) {
            throw std::runtime_error("invalid command line");
        }
        std::ifstream ifs(argv[1]);
        if (!ifs) {
            throw std::runtime_error("can't open file");
        }
        malbolge_machine mm(ifs);
        while (!mm.exec_one_step(std::cin, std::cout)) {
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
