#include "malbolge_machine.hpp"
#include <iostream>
#include <fstream>
#include <stdexcept>

int main(int argc, char *argv[]) try
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
} catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
} catch (...) {
    std::cerr << "an unknown exception (a class not derived from std::exception) is caught." << std::endl;
    return EXIT_FAILURE;
}
