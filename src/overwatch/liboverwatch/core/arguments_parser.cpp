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

#include <iostream>
#include <regex>

#include "arguments_parser.hpp"

namespace overwatch::core
{
    namespace
    {
        std::string const help_str_ = "help";
        std::string const help_str_short_ = "h";
        std::string const help_option_ = help_str_ + "," + help_str_short_;
        std::string const help_description_ = "  Produces a help message";

        std::string const arp_str_ = "arpspoof";
        std::string const arp_str_short_ = "a";
        std::string const arp_option_ = arp_str_ + "," + arp_str_short_;
        std::string const arp_description_ = "  Redirects packets to sniff networking traffic (HOST is usually the local gateway)";

        std::string const interface_str_ = "interface";
        std::string const interface_str_short_ = "i";
        std::string const interface_option_ = interface_str_ + "," + interface_str_short_;
        std::string const interface_default_ = "eth0";
        std::string const interface_description_ = "  The interface to watch for network traffic";

        std::string const target_str_ = "target";
        std::string const target_str_short_ = "t";
        std::string const target_option_ = target_str_ + "," + target_str_short_;
        std::string const target_description_ = "  Specify a host to overwatch";

        std::string const logging_str_ = "logging";
        std::string const logging_str_short_ = "l";
        std::string const logging_option_ = logging_str_ + "," + logging_str_short_;
        std::string const logging_description_ = "  Logging output (logging_path:logging_level)";
        std::string const logging_default_ = ":info";
        std::string const logging_value_name_ = "PATH:LEVEL";

        std::string const cmd_usage_ = "usage: overwatch -" + target_str_short_ + " TARGET [-" + help_str_short_ + "] [-" + arp_str_short_ + " HOST] [-" +
                                       interface_str_short_ + " INTERFACE] [-" + logging_str_short_ + " " + logging_value_name_ + "]";
    } // namespace

    ArgumentsParser::ArgumentsParser()
        : args_descriptions_{"", 1024, 512}
    {
        // Generate our descriptions for the different options
        args_descriptions_.add_options()(
            help_option_.c_str(), help_description_.c_str())(
            arp_option_.c_str(), boost_po::value<std::string>(), arp_description_.c_str())(
            interface_option_.c_str(), boost_po::value<std::string>()->default_value(interface_default_), interface_description_.c_str())(
            target_option_.c_str(), boost_po::value<std::string>()->required(), target_description_.c_str())(
            logging_option_.c_str(), boost_po::value<std::string>()->default_value(logging_default_), logging_description_.c_str());
    }

    args_map_t ArgumentsParser::parse(int const &argc, char const *const *const &argv)
    {
        boost_po::variables_map v_map;
        try
        {
            // Parse the command line and store it into a variables v_map
            boost_po::store(boost_po::parse_command_line(argc, argv, args_descriptions_), v_map);
        }
        catch (std::exception &e)
        {
            // Fails if the argument for an option is missing
            std::string err_str = std::string(e.what()) + "\n" +
                                  get_usage_();
            throw std::runtime_error{err_str};
        }
        // Check if the help option is available
        if (v_map.count(help_str_) > 0)
        {
            std::string help_str_ = get_usage_() + "\n" + get_descriptions_();
            throw std::runtime_error{help_str_};
        }
        args_map_t args;
        try
        {
            // Check for required commands
            notify(v_map);
            args = boost_to_args_map_(v_map);
        }
        catch (std::exception &e)
        {
            // Fails if one the required commands is missing
            std::string err_str = std::string(e.what()) + "\n" +
                                  get_usage_();
            throw std::runtime_error{err_str};
        }
        return args;
    }

    args_map_t ArgumentsParser::boost_to_args_map_(boost_po::variables_map const &v_map)
    {
        args_map_t args;
        if (v_map.count(arp_str_) > 0)
        {
            std::string const arp_spoof_ip = v_map[arp_str_].as<std::string>();
            if (!valid_ip_arg_(arp_spoof_ip))
            {
                throw std::runtime_error{"arpspoof host must be a valid IP address"};
            }
            args[Argument::ArpSpoofHost] = arp_spoof_ip;
        }

        // Interface has a default value - no need to check the contents
        args[Argument::Interface] = v_map[interface_str_].as<std::string>();

        // Target is required - no need to check if contents exist
        std::string const target_ip = v_map[target_str_].as<std::string>();
        if (!valid_ip_arg_(target_ip))
        {
            throw std::runtime_error{"target host must be a valid IP address"};
        }
        args[Argument::Target] = target_ip;

        // Since logging has a default value no need to check the contents
        std::string logging_val = v_map[logging_str_].as<std::string>();
        args[Argument::Logging] = logging_val;
        return args;
    }

    bool ArgumentsParser::valid_ip_arg_(std::string const &ip_str) noexcept
    {
        std::regex ip_regex{"^(?:[0-9]{1,3}\\.){3}[0-9]{1,3}$"};
        return std::regex_match(ip_str, ip_regex);
    }

    std::string const ArgumentsParser::get_usage_() const noexcept
    {
        //  Return the command usage
        return cmd_usage_ + "\n";
    }

    std::string const ArgumentsParser::get_descriptions_() const noexcept
    {
        // Converts the descriptions obj to a string to display in the terminal
        std::ostringstream out_stream;
        out_stream << args_descriptions_;
        return out_stream.str();
    }
} // namespace overwatch::core
