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

#ifdef _WIN32
#include "windows.hpp"
#else
#include <unistd.h>
#endif
#include <signal.h>
#include <iostream>
#include <string>

#include "config.hpp"
#include "logging.hpp"
#include "argument_parser.hpp"

namespace
{
    /**
     * Clean up the overwatch instance once a signal is fired
     * 
     * @param[in] The signal
     */
    void cleanup_(int) noexcept
    {
        LOG_DEBUG << "Received external shutdown signal";
        overwatch::core::g_config.signal_shutdown();
    }

    /**
     * Waits for the external shutdown
     */
    void sleep_() noexcept
    {
        LOG_INFO << "Waiting for external shutdown signal...";
        while (!overwatch::core::g_config.is_shutdown())
        {
            LOG_DEBUG << "Shutdown signal is not set";
            sleep(1);
        }
    }

    /**
     * Initializes the signals for cleanup
     */
    void init_signals_() noexcept
    {
        LOG_DEBUG << "Initializing cleanup signals";
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
    int overwatch_(int const argc, char const *const *const argv) noexcept
    {
        int exit_code = EXIT_SUCCESS;
        // Parse the args and convert to a config
        overwatch::core::ArgumentParser arg_parser;
        try
        {
            arg_parser.parse_args(argc, argv);
            // Generate the overwatch configuration
            overwatch::core::g_config =
                overwatch::core::Config(
                    arg_parser.get<std::string>(ARG_TARGET),
                    arg_parser.get<std::string>(ARG_INTERFACE),
                    arg_parser.get<std::string>(ARG_LOGGING),
                    arg_parser.present<std::string>(ARG_ARPSPOOF_HOST));
            // Validate the newly generate config values
            overwatch::core::g_config.validate();
            // Set the logger to log at the specified output
            common::logging::set_logger(overwatch::core::g_config.get_logging());
            LOG_INFO << overwatch::core::g_config.to_string();
            LOG_INFO << "Running overwatch...";
            init_signals_();
            // @TODO: Main execution
            sleep_();
            //wait_on_threads_();
        }
        catch (std::exception const &e)
        {
            if (common::logging::logger_initialized())
            {
                LOG_ERROR << e.what() << std::endl;
            }
            // Argparse failure
            else
            {
                // Print error and usage
                std::cout << e.what() << std::endl
                          << std::endl
                          << arg_parser.get_usage() << std::endl;
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