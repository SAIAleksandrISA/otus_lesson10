#pragma once

#include "interfaces.h"
#include <vector>
#include <string>
#include <chrono>
#include <fstream>

namespace bulk
{
    class FileOutputHandler : public IBlockOutputHandler
    {
    public:
        FileOutputHandler(std::string suffix);

        void handleBlock(const std::vector<std::string>& commands,
            std::chrono::time_point<std::chrono::system_clock> timestamp,
            long long file_id) override;

    private:
        std::string m_suffix;
    };
}