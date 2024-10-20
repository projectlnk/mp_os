#include <not_implemented.h>

#include <fstream>
#include <map>
#include "../include/client_logger.h"
#include "logger.h"
#include <string>

std::map <std::string, std::pair<std::ofstream*, int>> client_logger::logger_info = std::map <std::string, std::pair<std::ofstream*, int>>();

client_logger::client_logger(std::map<std::string, unsigned char> file_info, std::string format)
    : file_info(file_info), format(format)
{
    for (auto& local_map : file_info)
    {
        if (local_map.first == "cerr")
        {
            continue;  // Пропускаем консольный вывод
        }

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

std::string client_logger::getFormatString(const std::string& text, logger::severity sev) const
{
    std::string formatted_string;
    std::string datetime = current_datetime_to_string();
    std::string date = datetime.substr(0, 10);
    std::string time = datetime.substr(11, 8);

    std::string severity_str = severity_to_string(sev);

    for (size_t i = 0; i < format.length(); ++i) 
    {
        if (format[i] == '%' && i + 1 < format.length()) 
        {
            switch (format[i + 1]) 
            {
            case 'd':
                formatted_string += date;
                break;
            case 't':
                formatted_string += time;
                break;
            case 's':
                formatted_string += severity_str;
                break;
            case 'm':
                formatted_string += text;
                break;
            default:
                formatted_string += format[i];
                break;
            }
            ++i; 
        }
        else 
        {
            formatted_string += format[i];
        }
    }

    return formatted_string;
}

logger const* client_logger::log(
    const std::string& text,
    logger::severity severity) const noexcept
{
    std::string formatted_message = getFormatString(text, severity);

    // Вывод в консоль
    auto it = file_info.find("cerr");
    if (it != file_info.end() && it->second & (1 << static_cast<int>(severity)))
    {
        std::cerr << formatted_message << std::endl;
        std::cout << formatted_message << std::endl;
    }

    // Вывод в файлы
    for (const auto& entry : file_info)
    {
        if (entry.first == "cerr") continue;

        auto it = logger_info.find(entry.first);
        if (it != logger_info.end())
        {
            std::ofstream* file_stream = it->second.first;
            if (file_stream && file_stream->is_open())
            {
                *file_stream << formatted_message << std::endl;
            }
            else
            {
                std::cerr << "Failed to log to file: " << entry.first << std::endl;
            }
        }
        else
        {
            std::cerr << "No logger setup for file: " << entry.first << std::endl;
        }
    }

    return this;
}
