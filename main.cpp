#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

#include "version.hpp"
#include "bulk.hpp"
#include "async.h"


int main(int argc, char const *argv[])
{
    if (argc < 2) {
        std::cerr << "Using: ./bulk <block_size>" << std::endl;
        return -1;
    }

    auto block_size = std::stoi(argv[1], nullptr, 10);
    auto context = connect(block_size);
    
    buffer_t cmds = {"cmd1", "cmd2", "{", "cmd3", "cmd4", "cmd5", "}", "cmd6"};
    receive(&cmds, cmds.size(), context);
    disconnect(std::move(context));

    return 0;
}
