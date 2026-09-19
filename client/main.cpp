#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <memory>
#include "async/async.h"

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: bulk <block_size>" << std::endl;
        return 1;
    }

    size_t blockSize = 0;
    try
    {
        blockSize = std::stoul(argv[1]);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: Invalid block size." << std::endl;
        return 1;
    }

    bulk::Context ctx = bulk::connect(blockSize);
    if (ctx == 0)
    {
        return 1;
    }

    std::string line;
    while (std::getline(std::cin, line))
    {
        if (line == "exit")
        {
            break;
        }

        line += "\n";
        bulk::receive(line.c_str(), line.length(), ctx);
    }

    bulk::disconnect(ctx);

    return 0;
}