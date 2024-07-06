#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

#include <boost/asio.hpp>

#include "bulk.hpp"
#include "async.h"

using boost::asio::ip::tcp;


class session
  : public std::enable_shared_from_this<session>
{
public:
    session(tcp::socket socket, context_t bulk)
        : socket_(std::move(socket)),
          bulk_{std::move(bulk)}
    {
    }

    void start()
    {
        do_read();
    }

private:
    void do_read()
    {
    auto self = shared_from_this();
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
        [this, self](boost::system::error_code ec, std::size_t length)
        {
            if (!ec)
            {
                buffer_t buf = make_buffer(data_, length);
                receive(&buf, buf.size(), bulk_);

                //std::cout << "receive " << length << " = " << std::string{data_, length} << std::endl;
                do_write(length);
            }
        });
    }

    void do_write(std::size_t length)
    {
        auto self = shared_from_this();
        boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
            [this, self](boost::system::error_code ec, std::size_t /*length*/)
            {
                if (!ec)
                {
                    do_read();
                }
            });
    }

    buffer_t make_buffer(char* data, std::size_t length) {
        buffer_t buf;
        int start = 0;

        for (int i = 0;  i < length; ++i) {
            if (data[i] != '\n') {
                continue;
            }

            bulk::command_t cmd;
            std::copy(data + start, data + i, std::back_inserter(cmd));
            buf.push_back(cmd);
            start = i + 1;
        }

        return buf;
    }

    tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];

    context_t bulk_ = nullptr;
};

class server
{
public:
    server(boost::asio::io_context& io_context, short port, std::size_t block_size)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
      block_size_{block_size}
    {
        do_accept();
    }

private:
    void do_accept()
    {
    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket)
        {
            if (!ec)
            {
                context_t bulk = connect(block_size_);
                std::make_shared<session>(std::move(socket), std::move(bulk))->start();
            }

            do_accept();
        });
    }

    tcp::acceptor acceptor_;
    std::size_t block_size_ = 0;
};


int main(int argc, char const *argv[])
{
    if (argc < 3) {
        std::cerr << "Using: ./bulk_server <port> <block_size>" << std::endl;
        return -1;
    }

    auto port = std::stoi(argv[1], nullptr, 10);
    auto block_size = std::stoi(argv[2], nullptr, 10);

    try
    {
        boost::asio::io_context io_context;

        server server(io_context, port, block_size);

        io_context.run();
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Exception: " << ex.what() << "\n";
    }


    return 0;

    // auto context = connect(block_size);
    
    // buffer_t cmds = {"cmd1", "cmd2", "{", "cmd3", "cmd4", "cmd5", "}", "cmd6"};
    // receive(&cmds, cmds.size(), context);
    // disconnect(std::move(context));

}
