#include "client_session.h"
#include "core/connection_manager.h"
#include "threading/thread_manager.h"
#include <iostream>

namespace bulk
{
    ClientSession::ClientSession(boost::asio::ip::tcp::socket socket, size_t bulk_size)
        : m_socket(std::move(socket))
    {
        ThreadManager& tm = ConnectionManager::instance().get_tm();
        m_processor = std::make_unique<BulkProcessor>(
            static_cast<int>(bulk_size),
            tm.get_log_queue(),
            tm.get_shared_file_queue()
        );
    }

    ClientSession::~ClientSession()
    {
        m_processor->processEOF();
    }

    void ClientSession::start()
    {
        do_read();
    }

    void ClientSession::do_read()
    {
        auto self(shared_from_this());
        boost::asio::async_read_until(m_socket, m_buffer, '\n',
            [this, self](boost::system::error_code ec, std::size_t length)
            {
                handle_read(ec, length);
            });
    }

    void ClientSession::handle_read(boost::system::error_code ec, std::size_t length)
    {
        if (!ec)
        {
            std::istream is(&m_buffer);
            std::string line;
            std::getline(is, line);
            line += "\n";

            m_processor->processBuffer(line.c_str(), line.length());

            do_read();
        }
    }
}