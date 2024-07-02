#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

#include <boost/asio.hpp>

// #include "version.hpp"
// #include "bulk.hpp"
// #include "async.h"

using boost::asio::ip::tcp;


int main(int argc, char const *argv[])
{
    if (argc < 3) {
        std::cerr << "Using: ./bulk_server <port> <block_size>" << std::endl;
        return -1;
    }

    auto port = std::stoi(argv[1], nullptr, 10);
    auto block_size = std::stoi(argv[2], nullptr, 10);

    try {
        boost::asio::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), port));

        for (;;) {
            tcp::socket socket(io_context);
            acceptor.accept(socket);

            std::string answer = "Hello world";

            boost::system::error_code error;
            boost::asio::write(socket, boost::asio::buffer(answer), error);
        }
    } catch(std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;

    // auto context = connect(block_size);
    
    // buffer_t cmds = {"cmd1", "cmd2", "{", "cmd3", "cmd4", "cmd5", "}", "cmd6"};
    // receive(&cmds, cmds.size(), context);
    // disconnect(std::move(context));

    return 0;
}
