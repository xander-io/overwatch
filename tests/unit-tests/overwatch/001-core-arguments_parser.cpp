#include <regex>

#include "catch.hpp"
#include "arguments_parser.hpp"

#define TEST_NAME_PREFIX "ArgumentsParser::"

unsigned int const NUM_REQUIRED_ARGS = 3;

namespace
{
    void add_required_args_(char const **args)
    {
        args[0] = "overwatch";
        args[1] = "-c";
        args[2] = "~/Desktop/test.yml";
    }
} // namespace

TEST_CASE(TEST_NAME_PREFIX "Help displays usage with descriptions")
{
    int const argc = 2;
    char const *args[argc]{static_cast<char const *>("overwatch"), static_cast<char const *>("-h")};
    std::string const help_regex = " -h \\[ --help \\]\\s+Produces a help message";
    overwatch::core::ArgumentsParser parser;

    // Shortened version
    try
    {
        parser.parse(argc, args);
    }
    catch (std::exception const &e)
    {
        REQUIRE(std::regex_search(e.what(), std::regex(help_regex)));
    }

    // Long version
    args[1] = "--help";
    try
    {
        parser.parse(argc, args);
    }
    catch (std::exception const &e)
    {
        REQUIRE(std::regex_search(e.what(), std::regex(help_regex)));
    }
}
