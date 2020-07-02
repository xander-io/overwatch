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

#include <sys/stat.h>
#include <filesystem>

#include "config.hpp"

namespace overwatch::core
{
    Config::Config(args_map_t const &args)
        : target_ip{std::any_cast<std::string>(args.at(Argument::Target))},
          iface{std::any_cast<std::string>(args.at(Argument::Interface))},
          logging{std::any_cast<std::string>(args.at(Argument::Logging))},
          arpspoof_host_ip{(args.find(Argument::ArpSpoofHost) == args.end())
                               ? nullptr
                               : new std::string{std::any_cast<std::string>(args.at(Argument::ArpSpoofHost))}},
          shutdown{false}
    {
    }

    std::string const Config::to_string() noexcept
    {
        // Get all string values from the arguments map
        std::string config_str = "";
        // Dynamically calculate the banner size
        std::string disabled_value = "DISABLED";
        char const banner_symbol = '=';
        std::string const banner_title = " ( OVERWATCH CONFIGURATION ) ";
        // This equates to 6 sybmols on each side of the title
        size_t const min_banner_symbols = 24;

        // Determine parameter values
        std::string const *const target_ip_value = &target_ip;
        std::string const *const arpspoof_host_ip_value = arpspoof_host_ip ? arpspoof_host_ip.get() : &disabled_value;
        std::string const *const interface_value = &iface;
        std::string const *const logging_value = &logging;

        // Determine which value given is the largest
        size_t const max_value_size =
            std::max({target_ip_value->length(), arpspoof_host_ip_value->length(),
                      interface_value->length(), logging_value->length()});
        size_t const total_banner_symbols = std::max(min_banner_symbols, max_value_size);

        // Display banner with current configuration for overwatch
        std::string const padding{10, ' '};
        std::string const top_banner{std::string(total_banner_symbols / 2, banner_symbol) + banner_title +
                                     std::string(total_banner_symbols / 2, banner_symbol)};
        std::string const bottom_banner{std::string(top_banner.length(), banner_symbol)};
        config_str += "\n\t\t" + top_banner + "\n";
        config_str += "\t\t\tTarget IP: \t\t" + *target_ip_value + "\n";
        config_str += "\t\t\tArpspoof Host IP: \t" + *arpspoof_host_ip_value + "\n";
        config_str += "\t\t\tInterface: \t\t" + *interface_value + "\n";
        config_str += "\t\t\tLogging: \t\t" + *logging_value + "\n";
        config_str += "\t\t" + bottom_banner;
        return config_str;
    }
} // namespace overwatch::core