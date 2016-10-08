#include "util.hpp"
#include "Ipros.hpp"

namespace Ipros {
    using boost::asio::ip::tcp;

    std::ostream& operator<<(std::ostream& os, const ConnBuf& b) {
        hexdump(os, b.begin(), b.end());
        return os;
    }

    Conn::Conn(tcp::socket socket): socket_(std::move(socket)) {
    }

    void Conn::start() {
        do_read();
    }

    void Conn::do_read() {
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

    void Conn::do_write(std::size_t length) {
        auto self(shared_from_this());
        boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
                [this, self](boost::system::error_code ec, std::size_t length) {
                    if (ec) {
                        std::cout << "write err" << ec << std::endl;
                        do_read();
                        return;
                    }
                    std::cout << "out message: " << length << ":" << std::endl
                        << data_ << std::endl;
                    do_read();
                });
    }

    Bind::Bind(boost::asio::io_service& io_service, short port, Processor* proc):
        acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
        socket_(io_service),
        proc_(proc)
    {
        do_accept();
    }

    void Bind::do_accept() {
        acceptor_.async_accept(socket_,
                [this](boost::system::error_code ec) {
                    if (ec) {
                        std::cout << "accept error: " << ec << std::endl;
                    } else {
                        std::shared_ptr<Conn> cn = std::make_shared<Conn>(std::move(socket_));
                        if (proc_)
                            proc_->on_accept(cn);
                        cn->start();
                    }
                    do_accept();
                });
    }

    Req::Req(std::shared_ptr<Conn> cn): cn_(cn) {
    }
}
