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