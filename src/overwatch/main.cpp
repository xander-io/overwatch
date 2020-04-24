// #include <iostream>
// #include <filesystem>
// #include <boost/exception/all.hpp>
// #include <thread>

// #include "arguments_parser.hpp"
#include <thread>
#include <future>
#include <iostream>

#include "start.hpp"

int main(int const argc, char const *const *const argv)
{
    // try
    // {
    std::cout << "WE ARE HERE" << std::endl;
    //std::future<int> err_code = std::async(overwatch::start, argc, argv);
    //return err_code.get();

    // Create a base logger for logging arg parsing issues
    // jarvis::utils::init_logger();
    // LOG_DEBUG << "Logger communications intialized.";
    // // Parse the args and convert to a config
    // jarvis::parser::args_map_t const args = parse_args(argc, argv);
    // LOG_DEBUG << "Extracting configuration data.";
    // std::string const collection_path = std::any_cast<std::string>(args.at(jarvis::parser::Argument::Collection));
    // std::string const logging_formatted_str = std::any_cast<std::string>(args.at(jarvis::parser::Argument::Logging));
    // int const interval = std::any_cast<int>(args.at(jarvis::parser::Argument::Interval));
    // jarvis::core::Config const config{collection_path, logging_formatted_str, interval};
    // // Run jarvis
    // LOG_INFO << "Initializing Jarvis...";
    // jarvis::core::Jarvis jarvis{config};
    // jarvis.run();
    // }
    // catch (std::exception &e)
    // {
    //     // Would only occur if base logger fails initialization
    //     if (!jarvis::utils::logger_initialized())
    //     {
    //         jarvis::utils::init_logger();
    //     }
    //     LOG_ERROR << e.what();
    //     return EXIT_FAILURE;
    // }
    // return EXIT_SUCCESS;
}