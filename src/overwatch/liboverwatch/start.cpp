#include <iostream>
#include "start.hpp"
#include "overwatch_instance.hpp"

namespace overwatch
{
int start(int const &argc, char const *const *const &argv)
{
    overwatch::core::OverwatchInstance instance;
    return instance.run(argc, argv);
}
} // namespace overwatch