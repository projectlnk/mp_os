#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H

#include <logger.h>
#include <map>
#include "client_logger_builder.h"

class client_logger final :
    public logger
{

    friend class client_logger_builder;
private:

    static std::map <std::string, std::pair<std::ofstream*, int>> logger_info;
    std::map<std::string, unsigned char> file_info;
    std::string format;

private:

    std::string getFormatString(const std::string& text,
        logger::severity severity);
private:
    client_logger(std::map<std::string, unsigned char> file_info, std::string file_name);

public:

    client_logger(
        client_logger const& other);

    client_logger& operator=(
        client_logger const& other);

    client_logger(
        client_logger&& other) noexcept;

    client_logger& operator=(
        client_logger&& other) noexcept;

    ~client_logger() noexcept final;

public:

    [[nodiscard]] logger const* log(
        const std::string& message,
        logger::severity severity) const noexcept override;

};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H