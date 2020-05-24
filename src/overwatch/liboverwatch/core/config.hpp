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

#pragma once

#include <string>
#include <filesystem>

#include "arguments_parser.hpp"

namespace overwatch::core
{
    /**
     * Holds the configuration data for the overwatch instance
     */
    class Config
    {
    public:
        /**
         * Constructor for the overwatch config
         * 
         * @param[in] args Argument map used to convert to a configuration object
         */
        Config(args_map_t const &args);
        /**
         * Converts the config object to a printable string
         *
         * @return The confiugration in string format
         */
        std::string const to_string() noexcept;

        //////// Required configuration options ////////
        // Target IP address to watch for networking activies
        std::string const target_ip;
        // Interface to be listening for networking activies
        std::string const interface;
        // Logging format
        std::string const logging;
        ////////////////////////////////////////////////

        //////// Optional configuration options ////////
        // Arpspoof IP to mimic the host and redirect network packets
        std::unique_ptr<std::string> const arpspoof_host_ip;
        ////////////////////////////////////////////////

        // Shutdown signal
        std::atomic_bool shutdown;
    };
} // namespace overwatch::core