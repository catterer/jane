#include <boost/asio.hpp>

namespace Ipros {
    using boost::asio::ip::tcp;
    class ConnBuf: public std::array<char, 1024>
    {
    public:
        friend std::ostream& operator<<(std::ostream& os, const ConnBuf& b);
    };

    class Conn: public std::enable_shared_from_this<Conn>
    {
    public:
        Conn(tcp::socket socket);
        void start();
    private:
        void do_read();
        void do_write(std::size_t length);
        tcp::socket socket_;
        ConnBuf data_;
    };

    class Req {
    public:
        Req(std::shared_ptr<Conn> cn);
    private:
        std::shared_ptr<Conn> cn_;
    };

    class Processor {
    public:
        virtual void on_accept(std::shared_ptr<Conn> cn) = 0;
        virtual void on_msg(Conn& cn, Req& req) = 0;
    };

    class Bind
    {
    public:
        Bind(boost::asio::io_service& io_service, short port, Processor* proc);
    private:
        void do_accept();
        tcp::acceptor               acceptor_;
        tcp::socket                 socket_;
        Processor*                  proc_;
    };
}
