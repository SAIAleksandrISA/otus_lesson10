#include "fileoutputhandler.h"
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <fstream>

namespace bulk
{

    std::string format_timestamp(const std::chrono::time_point<std::chrono::system_clock>& ts)
    {
        std::time_t time_now = std::chrono::system_clock::to_time_t(ts);
        std::tm tm_struct = *std::localtime(&time_now);

        auto duration = ts.time_since_epoch();
        auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration) % 1000;

        std::stringstream ss;
        ss << std::put_time(&tm_struct, "%Y%m%d_%H%M%S");
        ss << std::setfill('0') << std::setw(3) << millis.count();

        return ss.str();
    }

    FileOutputHandler::FileOutputHandler(std::string suffix)
        : m_suffix(std::move(suffix))
    {
    }

    void FileOutputHandler::handleBlock(const std::vector<std::string>& commands,
        std::chrono::time_point<std::chrono::system_clock> timestamp,
        long long file_id)
    {
        if (commands.empty())
        {
            return;
        }

        std::string filename = format_timestamp(timestamp);
        filename += m_suffix;

        std::stringstream ss_id;
        ss_id << std::setfill('0') << std::setw(5) << file_id;
        filename += "_" + ss_id.str();

        filename += ".log";

        std::ofstream outfile(filename, std::ios::app);

        if (!outfile.is_open())
        {
            std::cerr << "Error: Could not open file for writing: " << filename << std::endl;
            return;
        }

        for (size_t i = 0; i < commands.size(); ++i)
        {
            outfile << commands[i] << (i == commands.size() - 1 ? "" : ", ");
        }
        outfile << std::endl;

        outfile.close();
    }

}