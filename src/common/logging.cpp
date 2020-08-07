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

#include <time.h>
#include <string.h>
#include <atomic>
#include <fstream>
#include <stdexcept>
#include <memory>
#include <sstream>
#include <chrono>
#include <iostream>
#include <algorithm>


#include "logging.hpp"

namespace common::logging
{
    namespace
    {
        // Internal state of the logger
        typedef struct LoggerInternals
        {
            // Keeps track of initialization state
            std::atomic_bool initialized;
            // Max severity level to log
            LogSeverity max_severity;
            // File stream for outputting to a file
            std::unique_ptr<std::ofstream> fstream;
        } LoggerInternals;

        LoggerInternals internals_{false, LogSeverity::Unknown, {}};

        /**
         * Transforms a logging severity to a string
         * 
         * @param[in] log_severity The severity to convert to a string
         * @return The string equivalent of the logging severity
         */
        std::string
        log_severity_to_str_(LogSeverity const &log_severity)
        {
            std::string log_severity_str;
            switch (log_severity)
            {
            case LogSeverity::Debug:
                log_severity_str = "DEBUG";
                break;
            case LogSeverity::Info:
                log_severity_str = "INFO";
                break;
            case LogSeverity::Warning:
                log_severity_str = "WARNING";
                break;
            case LogSeverity::Error:
                log_severity_str = "ERROR";
                break;
            default:
                log_severity_str = "UNKNOWN";
            }
            return log_severity_str;
        }

        /**
         * Transforms a string to a logging severity
         * 
         * @param[in] log_severity_str The string to convert to a LogSeverity
         * @return The LogSeverity equivalent of the string
         * @see LogSeverity
         */
        LogSeverity str_to_log_severity_(std::string const &log_severity_str)
        {
            LogSeverity log_severity = LogSeverity::Unknown;
            if (log_severity_str == "debug")
            {
                log_severity = LogSeverity::Debug;
            }
            else if (log_severity_str == "info")
            {
                log_severity = LogSeverity::Info;
            }
            else if (log_severity_str == "warning")
            {
                log_severity = LogSeverity::Warning;
            }
            else if (log_severity_str == "error")
            {
                log_severity = LogSeverity::Error;
            }
            return log_severity;
        }

        /**
         * Parses a formatted logging string into its respective file path and log severity
         * 
         * @param[in] formatted_logging_str The string to convert to a LogSeverity
         * @param[out] file_path The file path parsed from the formatted logging string
         * @param[out] log_severity The logging severity parsed from the formatted logging string
         * @throw std::invalid_argument If the logging string is formatted incorrectly
         */
        void parse_formatted_log_string_(std::string const &formatted_logging_str, std::filesystem::path *file_path, LogSeverity *log_severity)
        {
            // This should never happen unless the developer makes a mistake
            if (!file_path || !log_severity)
            {
                throw std::invalid_argument{"At least one logging parameter must not be null"};
            }

            size_t delimiter_index = formatted_logging_str.find(":");
            if (delimiter_index == std::string::npos)
            {
                throw std::invalid_argument{"Logging string formatted incorrectly - Expected '<optional_logging_path>:<logging_severity>'"};
            }
            // First index is the logging file, second index is the severity
            if (delimiter_index > 0)
            {
                *file_path = formatted_logging_str.substr(0, delimiter_index);
            }
            std::string log_severity_str = formatted_logging_str.substr(delimiter_index + 1, formatted_logging_str.size());
            std::transform(log_severity_str.begin(), log_severity_str.end(),
                           log_severity_str.begin(), [](char c) { return static_cast<char>(std::tolower(c)); });

            LOG_DEBUG << "[LOGGING] = " << formatted_logging_str
                      << "; [FILE_PATH] = " << file_path->u8string()
                      << "; [SEVERITY] = " << log_severity_str;

            *log_severity = str_to_log_severity_(log_severity_str);
        }
    } // namespace

#ifdef NDEBUG
    LogEntry::LogEntry(LogSeverity const &entry_severity)
        : entry_severity_{entry_severity}, entry_{""}
    {
    }
#else
    LogEntry::LogEntry(LogSeverity const &entry_severity,
                       long const &line, std::string const &function)
        : entry_severity_{entry_severity}, entry_{""}, line_{line}, function_{function}
    {
    }
#endif

    LogEntry::~LogEntry()
    {
        if (internals_.initialized && !entry_.empty())
        {
            log_entry_();
        }
    }

    #define TIME_SIZE 26
    void LogEntry::log_entry_()
    {
        if (entry_severity_ >= internals_.max_severity)
        {
            std::stringstream fmt;
            time_t const sys_time = std::chrono::system_clock::to_time_t(
                std::chrono::system_clock::now());
            char time[TIME_SIZE];
            ctime_s(time, sizeof(time), &sys_time);
            // Remove the \n
            time[strlen(time) - 1] = '\0';
            fmt << "[" << time;
#ifdef NDEBUG
            // "[%TimeStamp%] %LogSeverity% - %Message%"
            fmt << "] ";
#else
            // "[%TimeStamp%:%Function%:%Line%] %LogSeverity% - %Message%"
            fmt << ":" << function_ << "():" << line_ << "] ";
#endif
            fmt << log_severity_to_str_(entry_severity_) << " - " << entry_;

            // Log to file
            if (internals_.fstream)
            {
                *internals_.fstream << fmt.str() << std::endl;
            }
            // Log to console
            else
            {
                std::cout << fmt.str() << std::endl;
            }
        }
        entry_ = "";
    }

    LogEntry &LogEntry::operator<<(std::string const &str)
    {
        entry_.append(str);
        return *this;
    }

    LogEntry &LogEntry::operator<<(char const &c)
    {
        entry_.push_back(c);
        return *this;
    }

    LogEntry &LogEntry::operator<<(int const &num)
    {
        entry_.append(std::to_string(num));
        return *this;
    }

    LogEntry &LogEntry::operator<<(double const &num)
    {
        entry_.append(std::to_string(num));
        return *this;
    }

    LogEntry &LogEntry::operator<<(float const &num)
    {
        entry_.append(std::to_string(num));
        return *this;
    }

    LogEntry &LogEntry::operator<<(LogSeverity const &entry_severity)
    {
        entry_.append(log_severity_to_str_(entry_severity));
        return *this;
    }

    LogEntry &LogEntry::operator<<(ostream_function const &)
    {
        log_entry_();
        return *this;
    }

    bool logger_initialized() noexcept
    {
        return internals_.initialized;
    }

    void set_logger(std::filesystem::path const &file_path, LogSeverity const &max_severity)
    {
        std::string file_path_str;
        try
        {
            LOG_DEBUG << "Switching logging to file '" << file_path_str << "' with severity '" << max_severity << "'";
            if (max_severity == LogSeverity::Unknown)
            {
                throw std::invalid_argument{"Invalid logging severity"};
            }
            file_path_str = file_path.empty() ? "console" : file_path.u8string();

            if (!file_path.empty() && std::filesystem::is_directory(file_path))
            {
                throw std::invalid_argument{"'" + file_path_str + "' is not a valid logging file"};
            }
        }
        catch (std::invalid_argument const &e)
        {
            // Error during the initialization phase - set the logger to something valid such that the error gets logged properly
            if (!internals_.initialized)
            {
                set_logger(std::filesystem::path{}, LogSeverity::Error);
            }
            throw e;
        }

        internals_.max_severity = max_severity;
        // File path means logging to a file
        if (!file_path.empty())
        {
            LOG_DEBUG << "Creating directories for " << file_path_str;
            std::filesystem::create_directories(file_path.parent_path());
            if (!std::filesystem::exists(file_path))
            {
                LOG_DEBUG << "File at path '" << file_path_str << "' does not exist... Creating!";
                std::ofstream empty_file;
                empty_file.open(file_path, std::ios::out);
                internals_.fstream = std::make_unique<std::ofstream>(std::move(empty_file));
            }
            else
            {
                internals_.fstream = std::make_unique<std::ofstream>(file_path);
            }
        }
        // No file path means log to console
        else if (internals_.fstream)
        {
            // Explicitly delete the internal raw pointer to log to console
            internals_.fstream.reset();
        }
        internals_.initialized = true;
    }

    void set_logger(std::string const &formatted_log_str)
    {
        try
        {
            std::filesystem::path path;
            LogSeverity severity = LogSeverity::Unknown;
            parse_formatted_log_string_(formatted_log_str, &path, &severity);
            set_logger(path, severity);
        }
        catch (std::runtime_error const &e)
        {
            // Error during the initialization phase - set the logger to something valid such that the error gets logged properly
            if (!internals_.initialized)
            {
                set_logger(std::filesystem::path{}, LogSeverity::Error);
            }
            throw e;
        }
    }
} // namespace common::logging
