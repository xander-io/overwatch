#include <iostream>
#include <stdio.h>
#include <string>

#include "overwatch_instance.hpp"
#include "arguments_parser.hpp"
#include "logging.hpp"

namespace overwatch::core
{
namespace
{
args_map_t const parse_args(int const &argc, char const *const *const &argv)
{
    LOG_DEBUG << "Parsing arguments passed in from terminal";
    ArgumentsParser args_parser;
    // Get jarvis' arg values
    return args_parser.parse(argc, argv);
}
} // namespace

OverwatchInstance::OverwatchInstance() {}

int OverwatchInstance::run(int const &argc, char const *const *const &argv) noexcept
{
    int exit_code = EXIT_SUCCESS;
    try
    {
        //Create a base logger for logging arg parsing issues
        common::logging::init_logging();
        // Parse the args and convert to a config
        //args_map_t const args = parse_args(argc, argv);
        // /* Spawn external threads here and join on them */
    }
    catch (std::exception const &e)
    {
        if (common::logging::logging_initialized())
        {
            LOG_ERROR << e.what() << std::endl;
        }
        exit_code = EXIT_FAILURE;
    }
    return exit_code;
}
} // namespace overwatch::core