#pragma once

#include <unordered_map>

// #include "arguments_parser.hpp"
// #include "config.hpp"

namespace overwatch::core
{
class OverwatchInstance
{
public:
    OverwatchInstance();
    int run(int const &argc, char const *const *const &argv) noexcept;

private:
    // Config config_;
};
} // namespace overwatch::core