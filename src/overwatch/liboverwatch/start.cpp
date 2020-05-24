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

#include <signal.h>
#include <iostream>

#include "start.hpp"
#include "overwatch_instance.hpp"

namespace overwatch
{
    namespace
    {
        // The single instance used in the overwatch application
        overwatch::core::OverwatchInstance instance;
        /**
         * Clean up the overwatch instance once a signal is fired
         * 
         * @param[in] The signal
         */
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