#include <iostream>
#include <stdio.h>
#include <string>

#include "overwatch_instance.hpp"
//#include "arguments_parser.hpp"
#include "logging.hpp"

namespace overwatch::core
{
// namespace
// {
// args_map_t const parse_args(int const &argc, char const *const *const &argv)
// {
//     LOG_DEBUG << "Parsing arguments from the current user.";
//     ArgumentsParser args_parser;
//     // Get jarvis' arg values
//     return args_parser.parse(argc, argv);
// }
// } // namespace

OverwatchInstance::OverwatchInstance() {}

int OverwatchInstance::run(int const &argc, char const *const *const &argv)
{
    int err_code = EXIT_SUCCESS;
    try
    {
        //Create a base logger for logging arg parsing issues
        common::logging::init_logger();
        // LOG_DEBUG << "Logger communications intialized.";
        // // Parse the args and convert to a config
        // args_map_t const args = parse_args(argc, argv);
        // /* Spawn external threads here and join on them */
    }
    catch (std::exception const &e)
    {
        err_code = EXIT_FAILURE;
    }
    return err_code;
}
} // namespace overwatch::core