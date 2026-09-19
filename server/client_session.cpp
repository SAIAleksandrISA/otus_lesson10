#include "client_session.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <istream>
#include <streambuf>

namespace bulk_server
{

    ClientSession::ClientSession(boost::asio::any_io_executor io_executor, size_t bulk_size,
        bulk::ThreadSafeQueue<bulk::Block>& log_queue,
        bulk::ThreadSafeQueue<bulk::Block>& file_queue)
        : m_socket(io_executor)
        , m_bulk_size(bulk_size)
        , m_bulk_processor(bulk_size, log_queue, file_queue)
    {
    }

    ClientSession::~ClientSession()
    {
    }

    void ClientSession::start()
    {
        if (m_is_closed)
        {
            return;
        }
        do_read();
    }

    void ClientSession::close()
    {
        if (m_is_closed)
        {
            return;
        }
        m_is_closed = true;

        m_bulk_processor.processEOF();

        boost::system::error_code ec;
        m_socket.shutdown(tcp::socket::shutdown_both, ec);
        m_socket.close(ec);
    }

    void ClientSession::do_read()
    {
        if (m_is_closed)
        {
            return;
        }

        boost::asio::async_read_until(m_socket, m_buffer, '\n',
            [self = shared_from_this()](const boost::system::error_code& error, std::size_t bytes_transferred)
            {
                self->handle_read(error, bytes_transferred);
            });
    }

    void ClientSession::handle_read(const boost::system::error_code& error, [[maybe_unused]] std::size_t bytes_transferred) // <-- днаюбэ [[maybe_unused]]
    {
        if (error == boost::asio::error::operation_aborted)
        {
            return;
        }

        if (!error)
        {
            std::string line;
            std::istream is(&m_buffer);

            std::getline(is, line);

            if (!line.empty() && line.back() == '\r')
            {
                line.pop_back();
            }

            if (!line.empty())
            {
                m_bulk_processor.processCommand(line);
            }

            do_read();
        }
        else if (error == boost::asio::error::eof)
        {
            m_bulk_processor.processEOF();
            close();
        }
        else
        {
            m_bulk_processor.processEOF();
            close();
        }
    }

}