#include <iostream>
#include <CLI.h>

void CLI::help() {
    std::cout << "HELP ME!!!" << std::endl;
}

int CLI::run(int argc, char* argv[]) {
    if (argc <= 1) {
        help();
        return 1;
    }
    std::string command = argv[1];
    std::cout << command << std::endl;

    return 0;
}