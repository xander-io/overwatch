/* 
 * Overwatch - Remote Network Traffic Identifier 
 * Copyright (C) 2020 xander-io
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <algorithm>

#include "config.hpp"
#include "utils.hpp"

namespace overwatch::core
{
    Config g_config;
    bool Config::shutdown_{false};

    Config::Config()
        : target_ip_{""}, iface_{""}, logging_{""},
          arpspoof_host_ip_{std::nullopt}
    {
    }

    Config::Config(std::string target_ip, std::string iface,
                   std::string logging, std::optional<std::string> arpspoof_host_ip)
        : target_ip_{target_ip}, iface_{iface},
          logging_{logging}, arpspoof_host_ip_{arpspoof_host_ip}
    {
    }

    void Config::operator=(Config const &&config)
    {
        target_ip_ = config.target_ip_;
        iface_ = config.iface_;
        logging_ = config.logging_;
        arpspoof_host_ip_ = config.arpspoof_host_ip_;
    }

    std::string Config::get_target_ip() noexcept
    {
        return target_ip_;
    }

    std::string Config::get_interface() noexcept
    {
        return iface_;
    }

    std::string Config::get_logging() noexcept
    {
        return logging_;
    }

    std::optional<std::string> Config::get_arpspoof_host_ip() noexcept
    {
        return arpspoof_host_ip_;
    }

    bool Config::is_shutdown() noexcept
    {
        return shutdown_;
    }

    void Config::signal_shutdown() noexcept
    {
        shutdown_ = true;
    }

    void Config::validate()
    {
        if (iface_.empty())
        {
            throw std::invalid_argument{"Missing configuration data - 'interface' not set"};
        }
        else if (logging_.empty())
        {
            throw std::invalid_argument{"Missing configuration data - 'logging' not set"};
        }
        else if (target_ip_.empty() || !common::utils::is_valid_ip_addr(target_ip_))
        {
            throw std::invalid_argument{"'target' address is not a valid IP format"};
        }
        else if (arpspoof_host_ip_ && !common::utils::is_valid_ip_addr(*arpspoof_host_ip_))
        {
            throw std::invalid_argument{"'arpspoof' address is not a valid IP format"};
        }
    }

#define OPTIONAL_DISABLED "DISABLED"
#define BANNER_TITLE " ( OVERWATCH CONFIGURATION ) "
#define BANNER_SYMBOL '='
#define MIN_BANNER_SYMBOLS 24UL

    std::string const Config::to_string() noexcept
    {
        // Get all string values from the arguments map
        std::string config_str = "";
        // Determine which value given is the largest
        size_t const max_value_size =
            std::max({target_ip_.length(), (arpspoof_host_ip_ ? (*arpspoof_host_ip_).length() : 0),
                      iface_.length(), logging_.length()});
        size_t const total_banner_symbols = std::max(MIN_BANNER_SYMBOLS, max_value_size);

        // Display banner with current configuration for overwatch
        std::string const padding{10, ' '};
        std::string const top_banner{std::string(total_banner_symbols / 2, BANNER_SYMBOL) + BANNER_TITLE +
                                     std::string(total_banner_symbols / 2, BANNER_SYMBOL)};
        std::string const bottom_banner{std::string(top_banner.length(), BANNER_SYMBOL)};
        config_str += "\n\t\t" + top_banner + "\n";
        config_str += "\t\t\tTarget IP: \t\t" + target_ip_ + "\n";
        config_str += "\t\t\tArpspoof Host IP: \t" + (arpspoof_host_ip_ ? *arpspoof_host_ip_ : OPTIONAL_DISABLED) + "\n";
        config_str += "\t\t\tInterface: \t\t" + iface_ + "\n";
        config_str += "\t\t\tLogging: \t\t" + logging_ + "\n";
        config_str += "\t\t" + bottom_banner;
        return config_str;
    }
} // namespace overwatch::core