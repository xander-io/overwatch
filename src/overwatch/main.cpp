#include <future>

#include "start.hpp"

int main(int const argc, char const *const *const argv)
{
    std::future<int> exit_code = std::async(overwatch::start, argc, argv);
    return exit_code.get();
}