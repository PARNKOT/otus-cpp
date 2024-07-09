#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

#include <boost/asio.hpp>

#include "db.h"

using boost::asio::ip::tcp;
using db::Person, db::Table, db::Database;
using database_t = Database<Table<Person>>;
using db_pointer = std::shared_ptr<database_t>; //std::unique_ptr<database_t>;

class session
  : public std::enable_shared_from_this<session>
{
public:
    session(tcp::socket socket, db_pointer db)
        : socket_(std::move(socket)),
          db_{db}
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
                std::string query{data_, length};
                auto sql_desc = db::sql::parse_query(query);

                std::string response;

                switch (sql_desc.cmd)
                {
                case db::sql::SqlCommand::INSERT:
                    response = db::sql::insert_to_db(*db_, sql_desc) ? "< OK" : "< ERR: cannot insert data";
                    break;
                case db::sql::SqlCommand::TRUNCATE:
                    response = db::sql::truncate_table(*db_, sql_desc) ? "< OK" : "< ERR: cannot truncate table";
                    break;
                case db::sql::SqlCommand::INTERSECTION:

                    break;
                case db::sql::SqlCommand::SYMMETRIC_DIFFERENCE:

                    break;
                default:
                    break;
                }

                do_write(response);
            }
        });
    }

    void do_write(std::string response)
    {
        auto self = shared_from_this();
        boost::asio::async_write(socket_, boost::asio::buffer(response),
            [this, self](boost::system::error_code ec, std::size_t /*length*/)
            {
                if (!ec)
                {
                    do_read();
                }
            });
    }

    tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];

    db_pointer db_ = nullptr;
};

class server
{
public:
    server(boost::asio::io_context& io_context, short port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
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
                if (db_ == nullptr) {
                    db_ = std::make_shared<database_t>();
                    
                    db_->create_table("A");
                    db_->create_table("B");
                }

                std::make_shared<session>(std::move(socket), db_)->start();
            }

            do_accept();
        });
    }

    tcp::acceptor acceptor_;
    db_pointer db_ = nullptr;
};


int main(int argc, char const *argv[])
{
    auto port = std::stoi(argv[1], nullptr, 10);

    try
    {
        boost::asio::io_context io_context;

        server server(io_context, port);

        io_context.run();
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Exception: " << ex.what() << "\n";
    }

    return 0;
}
