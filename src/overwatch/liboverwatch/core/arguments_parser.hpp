#pragma once

#include <boost/program_options.hpp>
#include <string>
#include <unordered_map>
#include <any>

namespace overwatch::core
{
namespace po = boost::program_options;
enum class Argument
{
    Help,
    Logging,
    Interval
};
using args_map_t = std::unordered_map<Argument, std::any>;

class ArgumentsParser
{
public:
    ArgumentsParser();
    args_map_t parse(int const &argc, char const *const *const &argv);

private:
    args_map_t convert_to_args_map_(po::variables_map const &map);
    std::string const get_usage_() const noexcept;
    std::string const get_descriptions_() const noexcept;

    po::options_description args_descriptions_;
};
} // namespace overwatch::core