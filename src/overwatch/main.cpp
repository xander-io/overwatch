/* 
 * Overwatch - Remote Network Traffic Identifier 
 * Copyright (C) 2020 xander-io
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <future>

#include "start.hpp"

/**
 * Entry point for the overwatch executable
 * 
 * @param[in] argc Number of arguments
 * @param[in] argv Argument values
 * @return If the app succeeds or fails
 */
int main(int const argc, char const *const *const argv)
{
    std::future<int> exit_code = std::async(overwatch::start, argc, argv);
    return exit_code.get();
}