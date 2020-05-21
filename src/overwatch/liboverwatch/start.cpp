#include <iostream>
#include <signal.h>

#include "start.hpp"
#include "overwatch_instance.hpp"

namespace overwatch
{
    namespace
    {
        overwatch::core::OverwatchInstance instance;
        void cleanup_(int) noexcept
        {
            instance.shutdown();
        }

    } // namespace

    int start(int const &argc, char const *const *const &argv) noexcept
    {
        signal(SIGHUP, cleanup_);
        signal(SIGINT, cleanup_);
        signal(SIGTERM, cleanup_);
        return instance.run(argc, argv);
    }
} // namespace overwatch