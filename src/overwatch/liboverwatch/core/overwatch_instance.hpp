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
    // OverwatchInstance(Jarvis const &) = delete;
    // OverwatchInstance &operator=(Jarvis const &) = delete;

    int run(int const &argc, char const *const *const &argv);
    // Config const get_config() noexcept;

private:
    // Config config_;
};
} // namespace overwatch::core