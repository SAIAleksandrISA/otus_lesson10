#include "server.h"
#include <algorithm>
#include <functional>

namespace bulk_server
{

    Server::Server(boost::asio::io_context& io_context, short port, size_t bulk_size,
        bulk::ThreadSafeQueue<bulk::Block>& log_queue,
        bulk::ThreadSafeQueue<bulk::Block>& file_queue)
        : m_io_context(io_context)
        , m_acceptor(io_context, tcp::endpoint(tcp::v4(), port))
        , m_bulk_size(bulk_size)
        , m_log_queue(log_queue)
        , m_file_queue(file_queue)
    {
        start_accept();
    }

    Server::~Server()
    {
    }

    void Server::start_accept()
    {
        auto new_session = std::make_shared<ClientSession>(
            m_acceptor.get_executor(),
            m_bulk_size,
            m_log_queue,
            m_file_queue
        );

        m_acceptor.async_accept(new_session->socket(),
            [this, new_session](const boost::system::error_code& error)
            {
                if (!error)
                {
                    new_session->start();
                    m_active_sessions.push_back(new_session);

                    m_active_sessions.erase(std::remove_if(m_active_sessions.begin(), m_active_sessions.end(),
                        [](const std::shared_ptr<ClientSession>& session)
                        {
                            return session.use_count() == 1;
                        }),
                        m_active_sessions.end());

                    start_accept();
                }
                else
                {
                    if (error != boost::asio::error::operation_aborted)
                    {
                        start_accept();
                    }
                }
            });
    }

}