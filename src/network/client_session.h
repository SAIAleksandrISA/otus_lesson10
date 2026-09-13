#pragma once

#include "core/bulkprocessor.h"
#include <boost/asio.hpp>
#include <memory>
#include <string>

namespace bulk
{
    class ClientSession : public std::enable_shared_from_this<ClientSession>
    {
    public:
        ClientSession(boost::asio::ip::tcp::socket socket, size_t bulk_size);
        ~ClientSession();

        void start();

    private:
        void do_read();
        void handle_read(boost::system::error_code ec, std::size_t length);

        boost::asio::ip::tcp::socket m_socket;
        boost::asio::streambuf m_buffer;
        std::unique_ptr<BulkProcessor> m_processor;
    };
}