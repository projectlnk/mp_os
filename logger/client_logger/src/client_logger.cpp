#include <not_implemented.h>

#include <fstream>
#include <map>
#include "../include/client_logger.h"

/*std::string getFormatString(const std::string& text,
    logger::severity severity)
{
    for (auto format_iterator : format)
        // current_datetime_to_string().substr(0, 10);
        // current_datetime_to_string().substr(10, 9);
}*/

std::map <std::string, std::pair<std::ofstream*, int>> client_logger::logger_info = std::map <std::string, std::pair<std::ofstream*, int>>();
client_logger::client_logger(std::map<std::string, unsigned char> file_info, std::string format) : file_info(file_info), format(format);
{
    for (auto& local_map : file_info)
    {
        if (logger_info.find(local_map.first) == logger_info.end())
        {
            std::ofstream* fstream = new std::ofstream(local_map.first, std::ios::app);
            logger_info[local_map.first] = { fstream, 1 };
        }
        else
        {
            auto logger_iterator = logger_info.find(local_map.first);
            ++(logger_iterator->second.second);
        }
    }
}

client_logger::client_logger(
    client_logger const& other) : format(other.format), file_info(other.file_info)
{
    for (auto& local_map : file_info)
    {
        auto logger_iterator = logger_info.find(local_map.first);
        ++logger_iterator->second.second;
    }
}

client_logger& client_logger::operator=(client_logger const& other)
{
    if (this != &other)
    {
        for (const auto& local_map : file_info)
        {
            auto& logger_iterator = logger_info[local_map.first];
            --logger_iterator.second;

            if (logger_iterator.second == 0)
            {
                logger_iterator.first->close();
                delete logger_iterator.first;
            }
        }

        format = other.format;
        file_info = other.file_info;

        for (const auto& local_map : file_info)
        {
            auto& logger_iterator = logger_info[local_map.first];
            ++(logger_iterator.second);
        }
    }

    return *this;
}


client_logger::client_logger(
    client_logger&& other) noexcept : file_info(std::move(other.file_info)), format(std::move(other.format))
{
    for (const auto& local_map : other.file_info)
    {
        auto& logger_iterator = logger_info[local_map.first];
        --logger_iterator.second;

        if (logger_iterator.second == 0)
        {
            logger_iterator.first->close();
            delete logger_iterator.first;
        }
    }
}

client_logger& client_logger::operator=(
    client_logger&& other) noexcept
{
    if (this != &other)
    {
        for (const auto& local_map : file_info)
        {
            auto& logger_iterator = logger_info[local_map.first];
            --logger_iterator.second;

            if (logger_iterator.second == 0)
            {
                logger_iterator.first->close();
                delete logger_iterator.first;
            }
        }

        format = std::move(other.format);
        file_info = std::move(other.file_info);

        for (const auto& local_map : file_info)
        {
            auto& logger_iterator = logger_info[local_map.first];
            ++logger_iterator.second;
        }
    }

    return *this;
}

client_logger::~client_logger() noexcept
{
    for (const auto& local_map : file_info)
    {
        auto& logger_iterator = logger_info[local_map.first];
        --logger_iterator.second;

        if (logger_iterator.second == 0)
        {
            logger_iterator.first->close();
            delete logger_iterator.first;
        }
    }
}

logger const* client_logger::log(
    const std::string& text,
    logger::severity severity) const noexcept
{

}