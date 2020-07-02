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

#ifdef _WIN32
#include "windows.hpp"
#endif
#include "config.hpp"
#include "logging.hpp"
#include "arguments_parser.hpp"

namespace
{
    std::unique_ptr<overwatch::core::Config> config_{nullptr};

    /**
     * Clean up the overwatch instance once a signal is fired
     * 
     * @param[in] The signal
     */
    void cleanup_(int) noexcept
    {
        if (config_)
        {
            LOG_DEBUG << "Received external shutdown signal";
            config_->shutdown = true;
        }
    }

    /**
     * Waits for the external shutdown
     */
    void sleep_() noexcept
    {
        LOG_INFO << "Waiting for external shutdown signal...";
        while (!config_->shutdown)
        {
            LOG_DEBUG << "Shutdown signal is not set";
            sleep(1);
        }
    }

    overwatch::core::args_map_t const parse_args_(int const &argc, char const *const *const &argv)
    {
        overwatch::core::ArgumentsParser arg_parser;
        return arg_parser.parse(argc, argv);
    }

    /**
     * Initializes the signals for cleanup
     */
    void init_signals_() noexcept
    {
        LOG_DEBUG << "Adding cleanup signals";
        signal(SIGINT, cleanup_);
        signal(SIGTERM, cleanup_);
    }

    /**
     * Entry point for the overwatch executable
     * 
     * @param[in] argc Number of arguments
     * @param[in] argv Argument values
     * @return If the exe succeeds or fails
     */
    int overwatch_(int const &argc, char const *const *const &argv) noexcept
    {
        int exit_code = EXIT_SUCCESS;
        try
        {
            // Parse the args and convert to a config
            overwatch::core::args_map_t const args = parse_args_(argc, argv);
            config_ = std::unique_ptr<overwatch::core::Config>{new overwatch::core::Config{args}};
            common::logging::set_logger(config_->logging);
            LOG_INFO << config_->to_string();
            LOG_INFO << "Running overwatch...";
            init_signals_();
            sleep_();
            //wait_on_threads_();
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
} // namespace

/**
 * Entry point for the overwatch executable
 * 
 * @param[in] argc Number of arguments
 * @param[in] argv Argument values
 * @return If the exe succeeds or fails
 */
int main(int const argc, char const *const *const argv)
{
    return overwatch_(argc, argv);
}