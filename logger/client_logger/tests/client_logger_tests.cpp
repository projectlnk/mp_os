#include <gtest/gtest.h>
#include "client_logger.h"
#include "logger.h"
#include "client_logger_builder.h"



int main(
    int argc,
    char *argv[])
{
    auto* build_logger_1 = new client_logger_builder;

    build_logger_1->add_file_stream("D:\what.txt", logger::severity::information);

    build_logger_1->set_format("%t %d %m %s");

    logger* logger_1 = build_logger_1->build();

    logger_1->log("Zeleniy Slonik", logger::severity::information);

    delete build_logger_1;
    delete logger_1;

    return 0;
}
