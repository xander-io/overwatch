#pragma once

#include <string>
#include <filesystem>

#include "arguments_parser.hpp"

namespace overwatch::core
{
    class Config
    {
    public:
        Config(args_map_t const &args);
        std::string const to_string() noexcept;

        // Arguments that always contain values
        std::string const target_ip;
        std::string const interface;
        std::string const logging;

        // Arguments with optional values
        std::unique_ptr<std::string> const arpspoof_host_ip;

        std::atomic_bool shutdown;
    };
} // namespace overwatch::core