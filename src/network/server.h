#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <set>

namespace bulk
{
    class ClientSession;

    class Server
    {
    public:
        Server(boost::asio::io_context& io_context, short port, size_t bulk_size);

    private:
        void do_accept();

        boost::asio::ip::tcp::acceptor m_acceptor;
        size_t m_bulk_size;
        std::set<std::shared_ptr<ClientSession>> m_sessions;
    };
}