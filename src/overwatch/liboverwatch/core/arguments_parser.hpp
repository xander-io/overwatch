#pragma once

#include <boost/program_options.hpp>
#include <string>
#include <unordered_map>
#include <any>

namespace overwatch::core
{
    namespace boost_po = boost::program_options;
    enum class Argument
    {
        ArpSpoofHost,
        Interface,
        Target,
        Logging,
    };

    using args_map_t = std::unordered_map<Argument, std::any>;
    class ArgumentsParser
    {
    public:
        ArgumentsParser();
        args_map_t parse(int const &argc, char const *const *const &argv);

    private:
        args_map_t boost_to_args_map_(boost_po::variables_map const &map);
        std::string const get_usage_() const noexcept;
        std::string const get_descriptions_() const noexcept;
        bool valid_ip_arg_(std::string const &ip_str) noexcept;

        boost_po::options_description args_descriptions_;
    };
} // namespace overwatch::core