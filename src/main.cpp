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
#include "Ipros.hpp"

class MyProc: public Ipros::Processor {
    void on_accept(std::shared_ptr<Ipros::Conn> cn) {
        std::cout << "accepted" << std::endl;
    }
    void on_msg(Ipros::Conn& cn, Ipros::Req& req) {
        std::cout << "msg" << std::endl;
    }
};

int main(int argc, char* argv[])
{
    MyProc proc;
    try
    {
        if (argc != 2)
        {
            std::cerr << "Usage: IprosBind <port>\n";
            return 1;
        }

        boost::asio::io_service io_service;
        Ipros::Bind b(io_service, std::atoi(argv[1]), &proc);
        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
