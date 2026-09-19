#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <vector>
#include <string>

#include "client_session.h"
#include "core/bulkprocessor.h"
#include "threading/thread_manager.h"
#include "common/thread_safe_queue.h"
#include "common/block.h"

using boost::asio::ip::tcp;

namespace bulk_server
{

    class Server
    {
    public:
        Server(boost::asio::io_context& io_context, short port, size_t bulk_size,
            bulk::ThreadSafeQueue<bulk::Block>& log_queue,
            bulk::ThreadSafeQueue<bulk::Block>& file_queue);

        ~Server();

        void start_accept();

    private:
        void handle_accept(std::shared_ptr<ClientSession> session, const boost::system::error_code& error, tcp::endpoint endpoint);

        boost::asio::io_context& m_io_context;
        tcp::acceptor m_acceptor;
        size_t m_bulk_size;

        bulk::ThreadSafeQueue<bulk::Block>& m_log_queue;
        bulk::ThreadSafeQueue<bulk::Block>& m_file_queue;

        std::vector<std::shared_ptr<ClientSession>> m_active_sessions;
    };

}