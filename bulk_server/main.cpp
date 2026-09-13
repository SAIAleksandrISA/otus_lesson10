#include "network/server.h"
#include "core/connection_manager.h"
#include <boost/asio.hpp>
#include <iostream>

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: bulk_server <port> <bulk_size>" << std::endl;
        return 1;
    }

    try
    {
        unsigned short port = static_cast<unsigned short>(std::stoi(argv[1]));
        size_t bulk_size = std::stoul(argv[2]);

        bulk::ConnectionManager::instance().initialize();

        boost::asio::io_context io_context;
        bulk::Server server(io_context, port, bulk_size);

        io_context.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}