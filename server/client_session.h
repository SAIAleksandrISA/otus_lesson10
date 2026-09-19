#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <vector>
#include <string>

#include "core/bulkprocessor.h"
#include "common/thread_safe_queue.h"
#include "common/block.h"

using boost::asio::ip::tcp;

namespace bulk_server
{

    class ClientSession : public std::enable_shared_from_this<ClientSession>
    {
    public:
        ClientSession(boost::asio::any_io_executor io_executor, size_t bulk_size,
            bulk::ThreadSafeQueue<bulk::Block>& log_queue,
            bulk::ThreadSafeQueue<bulk::Block>& file_queue);

        ~ClientSession();

        tcp::socket& socket()
        {
            return m_socket;
        }

        void start();

        void close();

    private:
        void do_read();

        void handle_read(const boost::system::error_code& error, std::size_t bytes_transferred);

        tcp::socket m_socket;
        boost::asio::streambuf m_buffer;
        size_t m_bulk_size;
        bulk::BulkProcessor m_bulk_processor;

        bool m_is_closed = false;
    };

}