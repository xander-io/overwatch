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
#include <optional>

namespace overwatch::core
{
    /**
     * Holds the configuration data for the overwatch instance
     */
    class Config
    {
    public:
        /**
         * Constructors for overwatch configurations
         */
        Config();
        Config(std::string target_ip, std::string iface,
               std::string logging, std::optional<std::string> arpspoof_host_ip);

        /**
         * Overloaded = operator to create a new config from the
         * existing global config
         * @param[in] config The new config to replace the existing one
         */
        void operator=(Config const &&config);

        // Getters and setters for config
        std::string get_target_ip() noexcept;
        std::string get_interface() noexcept;
        std::string get_logging() noexcept;
        std::optional<std::string> get_arpspoof_host_ip() noexcept;
        bool is_shutdown() noexcept;
        void signal_shutdown() noexcept;

        /**
         * Validates the config items
         */
        void validate();

        /**
         * Converts the config object to a printable string
         * @return The confiugration in string format
         */
        std::string const to_string() noexcept;

    private:
        //////////////// REQUIRED ////////////////
        // Target IP address to watch for networking activies
        std::string target_ip_;
        // Interface to be listening for networking activies
        std::string iface_;
        // Logging format
        std::string logging_;
        /////////////////////////////////////////

        //////////////// OPTIONAL ////////////////
        // Arpspoof IP to mimic the host and redirect network packets
        std::optional<std::string> arpspoof_host_ip_;
        //////////////////////////////////////////

        // Static shutdown signal for the entire instance
        static bool shutdown_;
    };

    // The global config for overwatch
    extern Config g_config;
} // namespace overwatch::core