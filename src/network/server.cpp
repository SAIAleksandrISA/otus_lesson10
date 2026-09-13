#include "server.h"
#include "client_session.h"
#include <iostream>

namespace bulk
{
    Server::Server(boost::asio::io_context& io_context, short port, size_t bulk_size)
        : m_acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
        , m_bulk_size(bulk_size)
    {
        do_accept();
    }

    void Server::do_accept()
    {
        m_acceptor.async_accept(
            [this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket)
            {
                if (!ec)
                {
                    auto session = std::make_shared<ClientSession>(std::move(socket), m_bulk_size);
                    m_sessions.insert(session);
                    session->start();
                }
                do_accept();
            });
    }
}