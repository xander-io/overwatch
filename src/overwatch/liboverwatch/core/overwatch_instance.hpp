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