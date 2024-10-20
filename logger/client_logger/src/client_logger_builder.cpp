#include <not_implemented.h>

#include <logger.h>
#include "../include/client_logger_builder.h"

client_logger_builder::client_logger_builder() = default;

client_logger_builder::client_logger_builder(
    client_logger_builder const& other) = default;

client_logger_builder& client_logger_builder::operator=(
    client_logger_builder const& other) = default;

client_logger_builder::client_logger_builder(
    client_logger_builder&& other) noexcept = default;

client_logger_builder& client_logger_builder::operator=(
    client_logger_builder&& other) noexcept = default;

client_logger_builder::~client_logger_builder() noexcept = default;

logger_builder* client_logger_builder::set_format(const std::string& format_str)
{
    if (format_str.empty())
    {
        this->format = "%d %t %m %s";
    }
    else
    {
        this->format = format_str;
    }
    return this;
}

logger_builder* client_logger_builder::add_file_stream(
    std::string const& stream_file_path,
    logger::severity severity)
{
    auto iterator = file_info.find(stream_file_path);
    auto severity_bit = 1 << static_cast<int>(severity);

    if (iterator != file_info.end())
    {
        if (!((iterator->second >> static_cast<int>(severity)) & 1))
        {
            iterator->second ^= severity_bit;
            return 0;
        }
    }

    file_info.emplace(stream_file_path, severity_bit);
    return this;
}

logger_builder* client_logger_builder::add_console_stream(
    logger::severity severity)
{
    auto iterator = file_info.find("cerr");
    auto severity_bit = 1 << static_cast<int>(severity);

    if (iterator != file_info.end())
    {
        if (!((iterator->second >> static_cast<int>(severity)) & 1))
        {
            iterator->second ^= severity_bit;
            return 0;
        }
    }

    file_info.emplace("cerr", severity_bit);
    return this;
}

logger_builder* client_logger_builder::transform_with_configuration(
    std::string const& configuration_file_path,
    std::string const& configuration_path)
{
    throw not_implemented("logger_builder* client_logger_builder::transform_with_configuration(std::string const &configuration_file_path, std::string const &configuration_path)", "your code should be here...");
}

logger_builder* client_logger_builder::clear()
{
    file_info.clear();
    return this;
}

logger* client_logger_builder::build() const
{
    return new client_logger(file_info, format);
}
