//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <array>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <type_traits>
#include <utility>
#include <boost/asio.hpp>
#include "util.hpp"

using boost::asio::ip::tcp;

class IprosPkt
{
private:
    uint32_t            msg;
    uint32_t            len;
    uint32_t            sync;
    std::vector<char>   body;
};

class ConnBuf: public std::array<char, 1024>
{
public:
    friend std::ostream& operator<<(std::ostream& os, const ConnBuf& b) {
        hexdump(os, b.begin(), b.end());
        return os;
    }
};

class TcpConn: public std::enable_shared_from_this<TcpConn>
{
public:
    TcpConn(tcp::socket socket):
        socket_(std::move(socket))
    {
    }

    void start() {
        do_read();
    }

private:
    void do_read() {
        auto self(shared_from_this());
        socket_.async_read_some(boost::asio::buffer(data_),
                [this, self](boost::system::error_code ec, std::size_t length) {
                    if (ec) {
                        std::cout << "read err: " << ec << std::endl;
                        return;
                    }
                    std::cout << "got message: " << length << ":" << std::endl
                        << data_ << std::endl;
                    do_write(length);
                });
    }

    void do_write(std::size_t length) {
        auto self(shared_from_this());
        boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
                [this, self](boost::system::error_code ec, std::size_t length) {
                    if (ec) {
                        std::cout << "write err" << ec << std::endl;
                        return;
                    }
                    std::cout << "out message: " << length << ":" << std::endl
                        << data_ << std::endl;
                    do_read();
                });
    }

    tcp::socket socket_;
    ConnBuf data_;
};

class TcpBind
{
public:
    TcpBind(boost::asio::io_service& io_service, short port)
        : acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
            socket_(io_service)
    {
        do_accept();
    }

private:
    void do_accept() {
        acceptor_.async_accept(socket_,
                [this](boost::system::error_code ec) {
                    if (!ec)
                        std::make_shared<TcpConn>(std::move(socket_))->start();
                    do_accept();
                });
    }

    tcp::acceptor acceptor_;
    tcp::socket socket_;
};

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 2)
        {
            std::cerr << "Usage: TcpBind <port>\n";
            return 1;
        }

        boost::asio::io_service io_service;
        TcpBind b(io_service, std::atoi(argv[1]));
        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
