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

#pragma once

#include <unordered_map>
#include <atomic>

#include "arguments_parser.hpp"
#include "config.hpp"

namespace overwatch::core
{
    /**
     * Main class that controls the overwatch instance
     */
    class OverwatchInstance
    {
    public:
        /**
         * Constructor for the overwatch instance
         */
        OverwatchInstance();
        /**
         * Runs the overwatch instance
         * @param[in] argc Number of arguments
         * @param[in] argv Argument values
         * @return If the instance succeeds or fails
         */
        int run(int const &argc, char const *const *const &argv) noexcept;
        /**
         * Shuts down the overwatch instance
         */
        void shutdown() noexcept;
        /**
         * Retrieves the config for the overwatch instance
         * 
         * @return The config
         */
        Config const get_config() noexcept;

    private:
        /**
         * Parses the args passed in from the user
         * 
         * @param[in] argc Number of arguments
         * @param[in] argv Argument values
         * @return The arguments in an unordered map
         */
        args_map_t const parse_args_(int const &argc, char const *const *const &argv);
        // FTODO: int init_*();
        /**
         * Causes the main thread to sleep waiting for the external shutdown to be set 
         */
        void sleep_();
        /**
         * Waits for the child threads to clean up once the shutdown signal has been set
         */
        void wait_on_threads_();

        // The config of the overwatch instance
        std::unique_ptr<Config> config_;
    };
} // namespace overwatch::core