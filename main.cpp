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
        return -1;
    }

    bulk::Bulk b(std::stoi(argv[1], nullptr, 10));
    b.execute();

    return 0;
}
