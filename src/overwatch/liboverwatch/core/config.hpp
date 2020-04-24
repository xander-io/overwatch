#pragma once

#include <string>
#include <filesystem>

#include "logging.hpp"

namespace overwatch::core
{
class Config
{
public:
    Config();
    Config(std::string const &collection_path, std::string const &logging, int const &interval);

    static std::string const to_string(Config const &config) noexcept;

    std::string get_collection_path() const noexcept;
    std::string get_logging() const noexcept;
    int get_interval() const noexcept;

private:
    std::string collection_path_;
    std::string logging_;
    int interval_;
};
} // namespace overwatch::core