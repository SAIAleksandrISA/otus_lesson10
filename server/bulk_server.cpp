#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cstdlib>

#include "server.h"
#include "core/connection_manager.h"
#include "threading/thread_manager.h"
#include "common/thread_safe_queue.h"
#include "common/block.h"

#ifdef _WIN32
#include <windows.h>
#endif

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    if (argc != 3)
    {
        std::cerr << "Usage: bulk_server <port> <bulk_size>" << std::endl;
        return 1;
    }

    unsigned short port = 0;
    try
    {
        port = static_cast<unsigned short>(std::stoi(argv[1]));
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: Invalid port '" << argv[1] << "'. " << e.what() << std::endl;
        return 1;
    }

    size_t bulk_size = 0;
    try
    {
        bulk_size = std::stoul(argv[2]);
        if (bulk_size == 0)
        {
            std::cerr << "Error: bulk_size must be greater than 0." << std::endl;
            return 1;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: Invalid bulk_size '" << argv[2] << "'. " << e.what() << std::endl;
        return 1;
    }

    try
    {
        boost::asio::io_context io_context;

        bulk::ConnectionManager::instance().initialize();

        bulk::ThreadSafeQueue<bulk::Block>& log_queue = bulk::ConnectionManager::instance().get_tm().get_log_queue();
        bulk::ThreadSafeQueue<bulk::Block>& file_queue = bulk::ConnectionManager::instance().get_tm().get_shared_file_queue();

        bulk_server::Server server(io_context, port, bulk_size, log_queue, file_queue);

        io_context.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
        bulk::ConnectionManager::instance().shutdown();
        return 1;
    }

    bulk::ConnectionManager::instance().shutdown();

    return 0;
}