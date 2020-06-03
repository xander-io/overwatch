#include <regex>
#include <any>
#include <string>

#include "catch.hpp"
#include "arguments_parser.hpp"

#define TEST_NAME_PREFIX "ArgumentsParser::"

namespace
{
    void add_required_args_(char const **argv, bool const &shortened_args)
    {
        argv[0] = "overwatch";
        argv[1] = shortened_args ? "-t" : "--target";
        argv[2] = "192.168.0.40";
    }
} // namespace

TEST_CASE(TEST_NAME_PREFIX "Help displays usage with descriptions")
{

    std::string const help_regex = " -h \\[ --help \\]\\s+Produces a help message";
    overwatch::core::ArgumentsParser parser;

    SECTION("Shortened help version displays usage")
    {
        int const argc = 2;
        char const *argv[argc]{static_cast<char const *>("overwatch"), static_cast<char const *>("-h")};
        try
        {
            parser.parse(argc, argv);
        }
        catch (std::exception const &e)
        {
            REQUIRE(std::regex_search(e.what(), std::regex(help_regex)));
        }
    }
    SECTION("Long help version displays usage")
    {
        int const argc = 2;
        char const *argv[argc]{static_cast<char const *>("overwatch"), static_cast<char const *>("--help")};
        try
        {
            parser.parse(argc, argv);
        }
        catch (std::exception const &e)
        {
            REQUIRE(std::regex_search(e.what(), std::regex(help_regex)));
        }
    }
    SECTION("Help displays even with valid arguments")
    {
        int const argc = 4;
        char const *argv[argc] = {};
        add_required_args_(argv, false);
        argv[argc - 1] = "--help";
        try
        {
            parser.parse(argc, argv);
        }
        catch (std::exception const &e)
        {
            REQUIRE(std::regex_search(e.what(), std::regex(help_regex)));
        }
    }
}

TEST_CASE(TEST_NAME_PREFIX "Required arguments are required to proceed")
{
    overwatch::core::ArgumentsParser parser;
    SECTION("Empty arguments fail")
    {
        int const argc = 1;
        char const *argv[argc]{static_cast<char const *>("overwatch")};
        REQUIRE_THROWS(parser.parse(argc, argv));
    }
    SECTION("ONLY non required arguments fail")
    {
        int const argc = 3;
        char const *argv[argc]{static_cast<char const *>("overwatch"), static_cast<char const *>("-a"),
                               static_cast<char const *>("192.168.0.42")};
        REQUIRE_THROWS(parser.parse(argc, argv));
    }
    SECTION("Required arguments pass")
    {
        int const argc = 3;
        char const *argv[argc] = {};
        add_required_args_(argv, true);
        REQUIRE_NOTHROW(parser.parse(argc, argv));
    }
}

TEST_CASE(TEST_NAME_PREFIX "IP address values are validated")
{
    overwatch::core::ArgumentsParser parser;
    int const argc = 3;
    char const *argv[argc] = {};
    add_required_args_(argv, true);

    SECTION("Invalid IP addresses fail")
    {
        // Just dots
        argv[argc - 1] = "...";
        REQUIRE_THROWS(parser.parse(argc, argv));

        // Missing one value
        argv[argc - 1] = "123.2.1";
        REQUIRE_THROWS(parser.parse(argc, argv));

        // Missing values but dots are correct
        argv[argc - 1] = "..523.123";
        REQUIRE_THROWS(parser.parse(argc, argv));

        // Regex
        argv[argc - 1] = "*.123.2.123";
        REQUIRE_THROWS(parser.parse(argc, argv));

        // More values than required
        argv[argc - 1] = "192.123.2.123.241";
        REQUIRE_THROWS(parser.parse(argc, argv));

        // Subnet
        argv[argc - 1] = "192.123.2.0/24";
        REQUIRE_THROWS(parser.parse(argc, argv));
    }
    SECTION("Valid IP succeeds")
    {
        argv[argc - 1] = "192.168.2.254";
        REQUIRE_NOTHROW(parser.parse(argc, argv));
    }
}

TEST_CASE(TEST_NAME_PREFIX "Argument values are available after parsing")
{
    overwatch::core::ArgumentsParser parser;
    SECTION("Default values work properly")
    {
        int const argc = 3;
        char const *argv[argc] = {};
        add_required_args_(argv, true);
        overwatch::core::args_map_t const args = parser.parse(argc, argv);
        // Check defaults are displayed
        REQUIRE(std::any_cast<std::string>(args.at(overwatch::core::Argument::Logging)) == ":info");
        REQUIRE(std::any_cast<std::string>(args.at(overwatch::core::Argument::Interface)) == "eth0");

        // Check optional isn't displayed
        REQUIRE(args.find(overwatch::core::Argument::ArpSpoofHost) == args.end());
    }
    SECTION("All arguments are retrievable")
    {
        int const num_args = 4;
        int const argc = num_args * 2 + 1;
        char const *argv[argc] = {};
        std::function<void(overwatch::core::args_map_t const &)> test_arg_values_ =
            [](overwatch::core::args_map_t const &map) {
                REQUIRE(std::any_cast<std::string>(map.at(overwatch::core::Argument::Target)) == "192.168.0.40");
                REQUIRE(std::any_cast<std::string>(map.at(overwatch::core::Argument::ArpSpoofHost)) == "192.168.0.2");
                REQUIRE(std::any_cast<std::string>(map.at(overwatch::core::Argument::Interface)) == "eth2");
                REQUIRE(std::any_cast<std::string>(map.at(overwatch::core::Argument::Logging)) == "/home/user/test.txt:debug");
            };

        add_required_args_(argv, true);
        argv[3] = "-a";
        argv[4] = "192.168.0.2";
        argv[5] = "-i";
        argv[6] = "eth2";
        argv[7] = "-l";
        argv[8] = "/home/user/test.txt:debug";
        overwatch::core::args_map_t const short_args = parser.parse(argc, argv);
        test_arg_values_(short_args);

        add_required_args_(argv, false);
        argv[3] = "--arpspoof";
        argv[5] = "--interface";
        argv[7] = "--logging";
        overwatch::core::args_map_t const long_args = parser.parse(argc, argv);
        test_arg_values_(long_args);
    }
}
