#pragma once

#include <unordered_map>
#include <atomic>

#include "arguments_parser.hpp"
#include "config.hpp"

namespace overwatch::core
{
    class OverwatchInstance
    {
    public:
        OverwatchInstance();
        int run(int const &argc, char const *const *const &argv) noexcept;
        void shutdown() noexcept;
        Config const get_config() noexcept;

    private:
        args_map_t const parse_args_(int const &argc, char const *const *const &argv);
        //void init_*();
        void sleep_();
        void wait_on_threads_();

        std::unique_ptr<Config> config_;
    };
} // namespace overwatch::core