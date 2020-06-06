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

#include <atomic>
#include <filesystem>
#include <fstream>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/attributes/mutable_constant.hpp>

#include "logging.hpp"

// Logging format used when setting up the logger
#ifdef NDEBUG
#define LOGGING_FORMAT "[%TimeStamp%] %LogSeverity% - %Message%"
#else
#define LOGGING_FORMAT "[%TimeStamp%:%Function%:%Line%] %LogSeverity% - %Message%"
#endif

namespace common::logging
{
    namespace boost_log = boost::log;

    namespace
    {
        // Internal bool to keep track of initialization state
        std::atomic_bool initialized = false;

        /**
         * Transforms a logging severity to a string
         * 
         * @param[in] log_severity The severity to convert to a string
         * @return The string equivalent of the logging severity
         */
        std::string log_severity_to_string_(LogSeverity const &log_severity)
        {
            std::string log_severity_str;
            switch (log_severity)
            {
            case LogSeverity::DEBUG:
                log_severity_str = "DEBUG";
                break;
            case LogSeverity::INFO:
                log_severity_str = "INFO";
                break;
            case LogSeverity::WARNING:
                log_severity_str = "WARNING";
                break;
            case LogSeverity::ERROR:
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
        LogSeverity string_to_log_severity_(std::string const &log_severity_str)
        {
            LogSeverity log_severity = LogSeverity::UNKNOWN;
            if (log_severity_str == "debug")
            {
                log_severity = LogSeverity::DEBUG;
            }
            else if (log_severity_str == "info")
            {
                log_severity = LogSeverity::INFO;
            }
            else if (log_severity_str == "warning")
            {
                log_severity = LogSeverity::WARNING;
            }
            else if (log_severity_str == "error")
            {
                log_severity = LogSeverity::ERROR;
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
            if (file_path == nullptr || log_severity == nullptr)
            {
                throw std::invalid_argument{"At least one logging parameter must not be null"};
            }

            size_t delimiter_index = formatted_logging_str.find(":");
            if (delimiter_index == std::string::npos)
            {
                throw std::invalid_argument{"Logging string formatted incorrectly"};
            }
            // First index is the logging file, second index is the severity
            if (delimiter_index > 0)
            {
                *file_path = std::filesystem::path{formatted_logging_str.substr(0, delimiter_index)};
            }
            std::string log_severity_str = formatted_logging_str.substr(delimiter_index + 1, formatted_logging_str.size());
            std::transform(log_severity_str.begin(), log_severity_str.end(),
                           log_severity_str.begin(), [](unsigned char c) { return std::tolower(c); });

            LOG_DEBUG << "[LOGGING] = " << formatted_logging_str
                      << "; [FILE_PATH] = " << file_path->u8string()
                      << "; [SEVERITY] = " << log_severity_str;

            *log_severity = string_to_log_severity_(log_severity_str);
        }

        /**
         * Sets the value for dynamically changing thread attributes for logging purposes.
         * Used in this scenario to add the line and function names
         * 
         * @tparam T the value of the attribute to set
         * @param[in] name The name of the attribute
         * @param[in] value The value of the attribute
         */
        template <typename T>
        void set_logging_thread_attribute_(std::string const &name, T const &value)
        {
            boost_log::attribute_set attr_set =
                boost_log::core::get()->get_thread_attributes();

            // Check to see if the value exists in the set
            if (!attr_set.count(name))
            {
                boost_log::core::get()->add_thread_attribute(name, boost_log::attributes::mutable_constant<T>(value));
            }
            else
            {
                boost_log::attributes::mutable_constant<T> attr =
                    boost_log::attribute_cast<boost_log::attributes::mutable_constant<T>>(attr_set[name]);
                attr.set(value);
            }
        }
    } // namespace

    LogEntry::LogEntry(LogSeverity const &log_severity)
        : log_severity_{log_severity}, entry_{""}
    {
        set_logging_thread_attribute_("LogSeverity", log_severity_to_string_(log_severity));
    }

#ifndef NDEBUG
    LogEntry::LogEntry(LogSeverity const &log_severity, long const &line, std::string const &func)
        : LogEntry(log_severity)
    {
        set_logging_thread_attribute_("Line", line);
        set_logging_thread_attribute_("Function", func + "()");
    }
#endif

    LogEntry::~LogEntry()
    {
        if (initialized && !entry_.empty())
        {
            log_entry_();
        }
    }

    void LogEntry::log_entry_()
    {
        switch (log_severity_)
        {
        case LogSeverity::DEBUG:
            BOOST_LOG_TRIVIAL(debug) << entry_;
            break;
        case LogSeverity::INFO:
            BOOST_LOG_TRIVIAL(info) << entry_;
            break;
        case LogSeverity::WARNING:
            BOOST_LOG_TRIVIAL(warning) << entry_;
            break;
        case LogSeverity::ERROR:
            BOOST_LOG_TRIVIAL(error) << entry_;
            break;
        default:
            throw std::runtime_error{"Unexpected Error -> Severity level not known to log entry"};
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

    LogEntry &LogEntry::operator<<(LogSeverity const &log_severity)
    {
        entry_.append(log_severity_to_string_(log_severity));
        return *this;
    }

    LogEntry &LogEntry::operator<<(ostream_function const &)
    {
        log_entry_();
        return *this;
    }

    bool logger_initialized() noexcept
    {
        return initialized;
    }

    void set_logger(std::filesystem::path const &file_path, LogSeverity const &max_severity)
    {
        boost_log::trivial::severity_level boost_severity;
        std::string file_path_str;
        try
        {
            LOG_DEBUG << "Switching logging to file '" << file_path_str << "' with severity '" << max_severity << "'";
            if (max_severity == LogSeverity::UNKNOWN)
            {
                throw std::invalid_argument{"Invalid logging severity"};
            }
            file_path_str = file_path.empty() ? "console" : file_path.u8string();

            if (!file_path.empty() && std::filesystem::is_directory(file_path))
            {
                throw std::invalid_argument{"'" + file_path_str + "' is not a valid logging file"};
            }

            switch (max_severity)
            {
            case LogSeverity::DEBUG:
                boost_severity = boost_log::trivial::debug;
                break;
            case LogSeverity::INFO:
                boost_severity = boost_log::trivial::info;
                break;
            case LogSeverity::WARNING:
                boost_severity = boost_log::trivial::warning;
                break;
            case LogSeverity::ERROR:
                boost_severity = boost_log::trivial::error;
                break;
            default:
                throw std::invalid_argument{"Logging severity not supported"};
            }
        }
        catch (std::invalid_argument const &e)
        {
            // Error during the initialization phase - set the logger to something valid such that the error gets logged properly
            if (!initialized)
            {
                set_logger(std::filesystem::path{}, LogSeverity::ERROR);
            }
            throw e;
        }

        // Remove all current logging sources
        boost_log::core::get()->remove_all_sinks();
        boost_log::add_common_attributes();
        if (file_path.empty())
        {
            boost_log::add_console_log(std::cout, boost_log::keywords::format = LOGGING_FORMAT,
                                       boost_log::keywords::auto_flush = true);
        }
        else
        {
            LOG_DEBUG << "Creating directories for " << file_path_str;
            std::filesystem::create_directories(file_path.parent_path());
            if (!std::filesystem::exists(file_path))
            {
                LOG_DEBUG << "File at path '" << file_path_str << "' does not exist... Creating!";
                std::ofstream empty_file;
                empty_file.open(file_path, std::ios::out);
                empty_file.close();
            }

            boost_log::add_file_log(file_path_str, boost_log::keywords::format = LOGGING_FORMAT,
                                    boost_log::keywords::auto_flush = true);
        }
        boost_log::core::get()->set_filter(boost_log::trivial::severity >= boost_severity);
        initialized = true;
    }

    void set_logger(std::string const &formatted_log_str)
    {
        try
        {
            std::filesystem::path path;
            LogSeverity severity = LogSeverity::UNKNOWN;
            parse_formatted_log_string_(formatted_log_str, &path, &severity);
            set_logger(path, severity);
        }
        catch (std::runtime_error const &e)
        {
            // Error during the initialization phase - set the logger to something valid such that the error gets logged properly
            if (!initialized)
            {
                set_logger(std::filesystem::path{}, LogSeverity::ERROR);
            }
            throw e;
        }
    }
} // namespace common::logging
