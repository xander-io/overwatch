#include <regex>

#include "utils.hpp"

namespace common::utils
{
    bool is_valid_ip_addr(std::string const &ip_addr) noexcept
    {
        std::regex ip_regex{"^(?:[0-9]{1,3}\\.){3}[0-9]{1,3}$"};
        return std::regex_match(ip_addr, ip_regex);
    }
} // namespace common::utils