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

#include <iostream>
#include <stdio.h>
#include <signal.h>
#include <string>

#include "overwatch_instance.hpp"
#include "logging.hpp"

namespace overwatch::core
{
    void OverwatchInstance::shutdown() noexcept
    {
        // If the config and logger have been initialized, activate the shutdown signal
        if (common::logging::logger_initialized() && config_)
        {
            LOG_INFO << "Shutting down overwatch instance";
            config_->shutdown = true;
        }
    }

    args_map_t const OverwatchInstance::parse_args_(int const &argc, char const *const *const &argv)
    {
        ArgumentsParser args_parser;
        return args_parser.parse(argc, argv);
    }

    void OverwatchInstance::sleep_()
    {
        LOG_INFO << "Waiting for external shutdown signal...";
        while (!config_->shutdown)
        {
            LOG_DEBUG << "Shutdown signal is not set";
            sleep(1);
        }
    }

    void OverwatchInstance::wait_on_threads_()
    {
        /* FTODO: Wait on threads to shutdown before continuing */
        return;
    }

    OverwatchInstance::OverwatchInstance()
        : config_{nullptr} {}

    int OverwatchInstance::run(int const &argc, char const *const *const &argv) noexcept
    {
        int exit_code = EXIT_SUCCESS;
        try
        {
            //Create a base logger for logging arg parsing issues
            // Parse the args and convert to a config
            args_map_t const args = parse_args_(argc, argv);
            config_ = std::unique_ptr<Config>{new Config{args}};
            common::logging::set_logger(config_->logging);
            LOG_INFO << config_->to_string();
            LOG_INFO << "Running overwatch...";
            sleep_();
            wait_on_threads_();
        }
        catch (std::exception const &e)
        {
            if (common::logging::logger_initialized())
            {
                LOG_ERROR << e.what() << std::endl;
            }
            // The logger will not be initialized if argparse fails
            else
            {
                std::cout << e.what() << std::endl;
            }
            exit_code = EXIT_FAILURE;
        }
        return exit_code;
    }

} // namespace overwatch::core