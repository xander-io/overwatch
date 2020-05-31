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
    /**
     * Valid arguments to parse from the user
     */
    enum class Argument
    {
        ArpSpoofHost,
        Interface,
        Target,
        Logging,
    };

    using args_map_t = std::unordered_map<Argument, std::any>;
    /**
     * Class used to parse arguments from the user
     */
    class ArgumentsParser
    {
    public:
        /**
         * Constructor for parsing arguments
         */
        ArgumentsParser();
        /**
         * Parses arguments from the user
         * @param[in] argc Number of arguments
         * @param[in] argv Argument values
         * @return Unordered map containing the arguments
         */
        args_map_t parse(int const &argc, char const *const *const &argv);

    private:
        /**
         * Converts the boost args map to a type of args_map_t using the
         * enum Argument as the keys
         * 
         * @param[in] map The boost variable map
         * @return An overwatch args map type
         * @throw std::runtime_exception if the values within the boost map are not valid
         */
        args_map_t boost_to_args_map_(boost_po::variables_map const &map);
        /**
         * Gets the usage of the arguments from the parser
         * 
         * @return Printable string containing the usage
         */
        std::string const get_usage_() const noexcept;
        /**
         * Retrieves the descriptions if the help program option is used
         * 
         * @return Printable string containing the descriptions of the program options
         */
        std::string const get_descriptions_() const noexcept;
        /** 
         * Determines whether the IP address is in a valid IP format
         * 
         * @param[in] ip_str The IP string to validate
         * @return Whether the IP string is in a valid IP format
         */
        bool valid_ip_arg_(std::string const &ip_str) noexcept;

        // Contains the descriptions of the program options
        boost_po::options_description args_descriptions_;
    };
} // namespace overwatch::core