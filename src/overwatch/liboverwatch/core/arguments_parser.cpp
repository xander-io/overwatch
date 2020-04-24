#include <iostream>
#include <boost/algorithm/string.hpp>

#include "arguments_parser.hpp"

namespace overwatch::core
{
namespace
{
std::string const help_str = "help";
std::string const help_str_short = "h";
std::string const help_option = help_str + "," + help_str_short;
std::string const help_description = "  Produces a help message";

std::string const time_interval_str = "interval";
std::string const time_interval_str_short = "i";
std::string const time_interval_option = time_interval_str + "," + time_interval_str_short;
std::string const time_interval_description = "  The number of seconds to wait before rerunning";
int const time_interval_default = -1;

std::string const logging_str = "logging";
std::string const logging_str_short = "l";
std::string const logging_option = logging_str + "," + logging_str_short;
std::string const logging_description = "  Logging output (logging_path:logging_level)";
std::string const logging_default = ":info";

std::string const path_value_name = "PATH";
std::string const logging_value_name = "PATH:LEVEL";
std::string const cmd_usage = "usage: jarvis [-" + help_str_short + "][-" + time_interval_str_short +
                              " TIME][-" + logging_str_short + " " + logging_value_name + "]";
} // namespace

ArgumentsParser::ArgumentsParser()
    : args_descriptions_{"", 1024, 512}
{
    // Generate our descriptions for the different options
    args_descriptions_.add_options()(
        time_interval_option.c_str(), po::value<int>()->default_value(time_interval_default), time_interval_description.c_str())(
        logging_option.c_str(), po::value<std::string>()->default_value(logging_default), logging_description.c_str())(
        help_option.c_str(), help_description.c_str());
}

args_map_t ArgumentsParser::parse(int const &argc, char const *const *const &argv)
{
    po::variables_map map;
    try
    {
        // Parse the command line and store it into a variables map
        po::store(po::parse_command_line(argc, argv, args_descriptions_), map);
    }
    catch (std::exception &e)
    {
        // Fails if the argument for an option is missing
        std::string err_str = "Error: " + std::string(e.what()) + "\n" +
                              get_usage_();
        throw std::runtime_error{err_str};
    }
    // Check if the help option is available
    if (map.count(help_str) > 0)
    {
        std::string help_str = get_usage_() + "\n" + get_descriptions_();
        throw std::runtime_error{help_str};
    }
    args_map_t args;
    try
    {
        // Check for required commands
        notify(map);
        args = convert_to_args_map_(map);
    }
    catch (std::exception &e)
    {
        // Fails if one the required commands is missing
        std::string err_str = "Error: " + std::string(e.what()) + "\n" +
                              get_usage_();
        throw std::runtime_error{err_str};
    }
    return args;
}

args_map_t ArgumentsParser::convert_to_args_map_(po::variables_map const &map)
{
    args_map_t args;
    // Since logging has a default value no need to check the contents
    std::string logging_val = map[logging_str].as<std::string>();
    boost::trim(logging_val);
    args[Argument::Logging] = logging_val;

    // Since interval has a default value no need to check the contents
    args[Argument::Interval] = map[time_interval_str].as<int>();
    return args;
}

std::string const ArgumentsParser::get_usage_() const noexcept
{
    //  Return the command usage
    return cmd_usage + "\n";
}

std::string const ArgumentsParser::get_descriptions_() const noexcept
{
    // Converts the descriptions obj to a string to display in the terminal
    std::ostringstream out_stream;
    out_stream << args_descriptions_;
    return out_stream.str();
}
} // namespace overwatch::core
