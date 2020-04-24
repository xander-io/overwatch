#include <sys/stat.h>
#include <filesystem>

#include "logging.hpp"
#include "config.hpp"

namespace overwatch::core
{
Config::Config(std::string const &collection_path, std::string const &logging, int const &interval)
    : collection_path_{collection_path}, logging_{logging}, interval_{interval}
{
}

std::string const Config::to_string(Config const &config) noexcept
{
    // Get all string values from the arguments map
    std::string config_str = "";
    std::string const collection_path_str = config.collection_path_;
    std::string const logging_str = config.logging_;
    std::string const interval_str = std::to_string(config.interval_);

    // Dynamically calculate the banner size
    char const banner_symbol = '=';
    std::string const banner_title = " ( OVERWATCH CONFIGURATION ) ";
    // This equates to 6 sybmols on each side of the title
    size_t const min_banner_symbols = 24;
    // Determine which value given is the largest
    size_t max_value_size = std::max({strlen(collection_path_str.c_str()), strlen(interval_str.c_str()), strlen(logging_str.c_str())});
    size_t const total_banner_symbols = std::max(min_banner_symbols, max_value_size);

    // Display banner with current configuration for overwatch
    std::string const padding{10, ' '};
    std::string const top_banner{std::string(total_banner_symbols / 2, banner_symbol) + banner_title +
                                 std::string(total_banner_symbols / 2, banner_symbol)};
    std::string const bottom_banner{std::string(strlen(top_banner.c_str()), banner_symbol)};
    config_str += top_banner + "\n";
    config_str += "Collection Path: \t" + collection_path_str + "\n";
    config_str += "Interval (seconds): \t" + interval_str + "\n";
    config_str += "Logging:         \t" + logging_str + "\n";
    config_str += bottom_banner;
    return config_str;
}

std::string Config::get_collection_path() const noexcept
{
    return collection_path_;
}

std::string Config::get_logging() const noexcept
{
    return logging_;
}

int Config::get_interval() const noexcept
{
    return interval_;
}

} // namespace overwatch::core