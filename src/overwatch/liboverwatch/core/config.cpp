#include <sys/stat.h>
#include <filesystem>

#include "config.hpp"

namespace overwatch::core
{
    Config::Config(args_map_t const &args)
        : target_ip{std::any_cast<std::string>(args.at(Argument::Target))},
          arpspoof_host_ip{(args.find(Argument::ArpSpoofHost) == args.end())
                               ? nullptr
                               : new std::string{std::any_cast<std::string>(args.at(Argument::ArpSpoofHost))}},
          interface{std::any_cast<std::string>(args.at(Argument::Interface))},
          logging{std::any_cast<std::string>(args.at(Argument::Logging))},
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
        std::string const *const interface_value = &interface;
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
        std::string const bottom_banner{std::string(strlen(top_banner.c_str()), banner_symbol)};
        config_str += "\n\t\t" + top_banner + "\n";
        config_str += "\t\t\tTarget IP: \t\t" + *target_ip_value + "\n";
        config_str += "\t\t\tArpspoof Host IP: \t" + *arpspoof_host_ip_value + "\n";
        config_str += "\t\t\tInterface: \t\t" + *interface_value + "\n";
        config_str += "\t\t\tLogging: \t\t" + *logging_value + "\n";
        config_str += "\t\t" + bottom_banner;
        return config_str;
    }
} // namespace overwatch::core