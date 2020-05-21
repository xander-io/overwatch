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