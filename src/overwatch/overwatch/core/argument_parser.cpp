#include <ostream>

#include "argument_parser.hpp"

#define ARG_ARPSPOOF_HOST_ABRV "-a"
#define ARG_INTERFACE_ABRV "-i"
#define ARG_LOGGING_ABRV "-l"

namespace overwatch::core
{
    ArgumentParser::ArgumentParser()
        : internal_parser_{ "overwatch", "0.0.1" }
    {
        init_parser_args_();
    }

    void ArgumentParser::parse_args(int const argc, char const *const *const argv)
    {
        internal_parser_.parse_args(argc, argv);
    }

    void ArgumentParser::init_parser_args_() noexcept
    {
        internal_parser_.add_argument(ARG_ARPSPOOF_HOST_ABRV, ARG_ARPSPOOF_HOST)
            .help("IP of host to intercept packets for (HOST is usually the local gateway)");
        internal_parser_.add_argument(ARG_INTERFACE_ABRV, ARG_INTERFACE)
            .help("The interface to watch for network traffic")
            .default_value(std::string{ "eth0" });
        internal_parser_.add_argument(ARG_LOGGING_ABRV, ARG_LOGGING)
            .help("Logging (fmt: '<optional_logging_path>:<logging_severity>')")
            .default_value(static_cast<std::string>(":info"));
        internal_parser_.add_argument(ARG_TARGET)
            .help("Target IP to overwatch")
            .required();
    }

    std::string ArgumentParser::get_usage() noexcept
    {
        std::ostringstream stream;
        stream << internal_parser_;
        return stream.str();
    }
} // namespace overwatch::core
