#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

#include "version.hpp"
#include "bulk.hpp"


int main(int argc, char const *argv[])
{
    if (argc < 2) {
        std::cerr << "Using: ./bulk <block_size>" << std::endl;
        return -1;
    }

    bulk::Bulk b(std::stoi(argv[1], nullptr, 10));

    auto console_printer = std::make_shared<bulk::ConsolePrinter>();
    auto file_printer = std::make_shared<bulk::FilePrinter>(std::filesystem::path("./test.txt"));


    // b.add_printer(console_printer);
    // b.add_printer(file_printer);
    b.add_printer(std::make_shared<bulk::AsyncPrinter>(console_printer));
    b.add_printer(std::make_shared<bulk::AsyncPrinter>(file_printer));
    b.execute();

    return 0;
}
