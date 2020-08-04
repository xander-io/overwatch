#include <regex>
#include <any>
#include <string>
#include <optional>
#include <catch2/catch.hpp>

#include "argument_parser.hpp"

#define TEST_NAME_PREFIX "ArgumentParser::"

TEST_CASE(TEST_NAME_PREFIX "Help displays usage with descriptions")
{

    std::string const help_regex = "Usage: overwatch";
    overwatch::core::ArgumentParser parser;
    std::string const usage = parser.get_usage();
    REQUIRE(std::regex_search(usage, std::regex(help_regex)));
}

TEST_CASE(TEST_NAME_PREFIX "Required arguments are required to proceed")
{
    overwatch::core::ArgumentParser parser;
    SECTION("Empty arguments fail")
    {
        int const argc = 1;
        char const *argv[argc]{static_cast<char const *>("overwatch")};
        REQUIRE_THROWS(parser.parse_args(argc, argv));
    }
    SECTION("Failure if required arguments not present")
    {
        int const argc = 3;
        char const *argv[argc]{static_cast<char const *>("overwatch"), static_cast<char const *>("-a"),
                               static_cast<char const *>("192.168.0.42")};
        REQUIRE_THROWS(parser.parse_args(argc, argv));
    }
    SECTION("Required arguments pass")
    {
        int const argc = 2;
        char const *argv[argc] = {};
        argv[0] = "overwatch";
        argv[1] = "192.168.0.2";
        REQUIRE_NOTHROW(parser.parse_args(argc, argv));
    }
}

TEST_CASE(TEST_NAME_PREFIX "Argument values are available after parsing")
{
    int const num_optional_args = 3;
    overwatch::core::ArgumentParser parser;
    SECTION("Default values work properly")
    {
        int const argc = 2;
        char const *argv[argc] = {};
        argv[0] = "overwatch";
        argv[1] = "192.168.0.2";
        parser.parse_args(argc, argv);
        // Check defaults are displayed
        REQUIRE(parser.get<std::string>(ARG_LOGGING) == ":info");
        REQUIRE(parser.get<std::string>(ARG_INTERFACE) == "eth0");

        // Check optional isn't displayed
        REQUIRE(parser.present<std::string>(ARG_ARPSPOOF_HOST) == std::nullopt);
    }

    SECTION("All arguments are retrievable")
    {
        int const argc = num_optional_args * 2 + 2;
        char const *argv[argc] = {};

        // Testing abbreviated versions
        argv[0] = "overwatch";
        argv[1] = "-a";
        argv[2] = "192.168.0.2";
        argv[3] = "-i";
        argv[4] = "eth2";
        argv[5] = "-l";
        argv[6] = "/home/user/test.txt:debug";
        argv[7] = "192.168.0.40";

        parser.parse_args(argc, argv);
        REQUIRE(parser.get<std::string>(ARG_TARGET) == "192.168.0.40");
        REQUIRE(*parser.present<std::string>(ARG_ARPSPOOF_HOST) == "192.168.0.2");
        REQUIRE(parser.get<std::string>(ARG_INTERFACE) == "eth2");
        REQUIRE(parser.get<std::string>(ARG_LOGGING) == "/home/user/test.txt:debug");
    }

    SECTION("All long arguments are retrievable")
    {
        int const argc = num_optional_args * 2 + 2;
        char const *argv[argc] = {};

        // Testing long versions
        parser = overwatch::core::ArgumentParser{};
        argv[0] = "overwatch";
        argv[1] = "--arpspoof";
        argv[2] = "192.168.0.2";
        argv[3] = "--interface";
        argv[4] = "eth2";
        argv[5] = "--logging";
        argv[6] = "/home/user/test.txt:debug";
        argv[7] = "192.168.0.40";

        parser.parse_args(argc, argv);
        REQUIRE(parser.get<std::string>(ARG_TARGET) == "192.168.0.40");
        REQUIRE(*parser.present<std::string>(ARG_ARPSPOOF_HOST) == "192.168.0.2");
        REQUIRE(parser.get<std::string>(ARG_INTERFACE) == "eth2");
        REQUIRE(parser.get<std::string>(ARG_LOGGING) == "/home/user/test.txt:debug");
    }
}
