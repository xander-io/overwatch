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

#include <string>
#include <filesystem>

#ifdef NDEBUG
#define LOG_DEBUG common::logging::LogEntry(common::logging::LogSeverity::DEBUG)
#define LOG_INFO common::logging::LogEntry(common::logging::LogSeverity::INFO)
#define LOG_WARNING common::logging::LogEntry(common::logging::LogSeverity::WARNING)
#define LOG_ERROR common::logging::LogEntry(common::logging::LogSeverity::ERROR)
#else
// Adding the line and function to debug logging output
#define LOG_DEBUG common::logging::LogEntry(common::logging::LogSeverity::DEBUG, __LINE__, __func__)
#define LOG_INFO common::logging::LogEntry(common::logging::LogSeverity::INFO, __LINE__, __func__)
#define LOG_WARNING common::logging::LogEntry(common::logging::LogSeverity::WARNING, __LINE__, __func__)
#define LOG_ERROR common::logging::LogEntry(common::logging::LogSeverity::ERROR, __LINE__, __func__)
#endif

namespace common::logging
{
    enum class LogSeverity
    {
        UNKNOWN = -1,
        DEBUG,
        INFO,
        WARNING,
        ERROR
    };

    typedef std::ostream &(*ostream_function)(std::ostream &);
    // Wrapper class to create entries for the boost logger
    class LogEntry
    {
    public:
        LogEntry(LogSeverity const &log_severity);
#ifndef NDEBUG
        // Additional constructor in debug mode that attaches function and line number
        LogEntry(LogSeverity const &log_severity, long const &line, std::string const &func);
#endif
        ~LogEntry();
        // Used for logging different types
        LogEntry &operator<<(std::string const &str);
        LogEntry &operator<<(char const &c);
        LogEntry &operator<<(int const &num);
        LogEntry &operator<<(double const &num);
        LogEntry &operator<<(float const &num);
        LogEntry &operator<<(LogSeverity const &log_severity);
        // Used for things like std::endl for logging
        LogEntry &operator<<(ostream_function const &func);

    private:
        void log_entry_();

        LogSeverity const log_severity_;
        std::string entry_;
    };

    bool logger_initialized() noexcept;
    void set_logger(std::string const &formatted_log_str);
    void set_logger(std::filesystem::path const &file_path, LogSeverity const &max_severity);
} // namespace common::logging
