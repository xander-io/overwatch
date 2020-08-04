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

#include <argparse/argparse.hpp>

// Positional args
#define ARG_TARGET "target"
// Optional args
#define ARG_ARPSPOOF_HOST "--arpspoof"
#define ARG_INTERFACE "--interface"
#define ARG_LOGGING "--logging"

namespace overwatch::core
{
    /**
     * Wrapper class using argparse
     */
    class ArgumentParser
    {
    public:
        /**
         * Constructor for parsing arguments
         */
        ArgumentParser();
        /**
         * Parses arguments from the user
         * @param[in] argc Number of arguments
         * @param[in] argv Argument values
         */
        void parse_args(int const argc, char const *const *const argv);
        /**
         * Gets an argument value from the parser
         * @param[in] arg The argument name
         * @return The value of the argument
         */
        template <typename T>
        T get(std::string const &arg);
        /**
         * Gets an optional argument value from the parser
         * @param[in] arg The argument name
         * @return The value of the argument within an std::optional
         */
        template <typename T>
        std::optional<T> present(std::string const &arg);
        /**
         * Displays the help usage
         * @return The help usage in string format
         */
        std::string get_usage() noexcept;

    private:
        /**
         * Initailizes the parser with the specified arguments
         */
        void init_parser_args_() noexcept;

        // Internal third-party parser
        argparse::ArgumentParser internal_parser_;
    };

    template <typename T>
    T ArgumentParser::get(std::string const &arg)
    {
        return internal_parser_.get<T>(arg);
    }

    template <typename T>
    std::optional<T> ArgumentParser::present(std::string const &arg)
    {
        return internal_parser_.present<T>(arg);
    }

} // namespace overwatch::core