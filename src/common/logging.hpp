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
#define LOG_DEBUG common::logging::LogEntry(common::logging::LogSeverity::Debug)
#define LOG_INFO common::logging::LogEntry(common::logging::LogSeverity::Info)
#define LOG_WARNING common::logging::LogEntry(common::logging::LogSeverity::Warning)
#define LOG_ERROR common::logging::LogEntry(common::logging::LogSeverity::Error)
#else
// Adding the line and function to debug logging output
#define LOG_DEBUG common::logging::LogEntry(common::logging::LogSeverity::Debug, __LINE__, __func__)
#define LOG_INFO common::logging::LogEntry(common::logging::LogSeverity::Info, __LINE__, __func__)
#define LOG_WARNING common::logging::LogEntry(common::logging::LogSeverity::Warning, __LINE__, __func__)
#define LOG_ERROR common::logging::LogEntry(common::logging::LogSeverity::Error, __LINE__, __func__)
#endif

namespace common::logging
{
    /**
     * Implementation of logging severities.
     * 
     * The logging severity is used to determine the verbosity of the logging output.
     */
    enum class LogSeverity
    {
        Unknown = -1,
        Debug,
        Info,
        Warning,
        Error
    };

    typedef std::ostream &(*ostream_function)(std::ostream &);
    /**
     * Implementation of a logging entry for the logger.
     * 
     * Wrapper class around boost::log - used to create entries for the boost logger
     * and keep track of entry state.
     */
    class LogEntry
    {
    public:
        /**
         * Constructor used to log an entry.
         * 
         * @param[in] log_severity The severity level that the log entry is associated with.
         */
        LogEntry(LogSeverity const &log_severity);
#ifndef NDEBUG
        /**
         * Additional constructor only in debug mode used to log an entry with the line and function.
         * 
         * @param log_severity The severity level that the log entry is associated with.
         * @param[in] line The current line where the log was written from.
         * @param[in] func The function that the log is contained in.
         */
        LogEntry(LogSeverity const &log_severity, long const &line, std::string const &func);
#endif
        /// Destructor for log entry - Used to determine when the log entry is complete
        ~LogEntry();
        /**
         * Appends a string value to the log entry.
         * 
         * @param[in] str The string to append.
         * @returns the ongoing LogEntry instance
         * @overload
         */
        LogEntry &operator<<(std::string const &str);
        /**
         * Appends a character value to the log entry.
         * 
         * @param[in] c The charater to append.
         * @return the ongoing LogEntry instance
         * @overload
         */
        LogEntry &operator<<(char const &c);
        /**
         * Appends an int value to the log entry.
         * 
         * @param[in] num The int to append.
         * @overload
         */
        LogEntry &operator<<(int const &num);
        /**
         * Appends a double value to the log entry.
         * 
         * @param[in] num The double to append.
         * @returns the ongoing LogEntry instance
         * @overload
         */
        LogEntry &operator<<(double const &num);
        /**
         * Appends a float value to the log entry.
         * 
         * @param[in] num The float to append.
         * @overload
         */
        LogEntry &operator<<(float const &num);
        /**
         * Converts the log severity to a string
         * and appends a log severity to the log entry.
         * 
         * @param[in] log_severity The log severity to append.
         * @returns the ongoing LogEntry instance
         * @overload
         */
        LogEntry &operator<<(LogSeverity const &log_severity);
        /**
         * Catches ostream functions like std::endl.
         * Writes the log entry to the output source
         * 
         * @param[in] The ostream function
         * @returns the ongoing LogEntry instance
         * @overload
         */
        LogEntry &operator<<(ostream_function const &);

    private:
        // Logs the entry to the output source.
        void log_entry_();

        // Internal state of the logging severity.
        LogSeverity const log_severity_;
        // Internal state of ongoing entry.
        std::string entry_;
    };

    /**
     * Determines if the logger has been initialized.
     * 
     * @returns  True if the logger has been initialized.
     */
    bool logger_initialized() noexcept;
    /**
     * Sets the logger to the specified formatter string
     * 
     * @param[in] formatted_log_str A string that specifies the logging severity and file
     * @throw std::invalid_argument if the logger could not be initialized due to incorrect arguments
     * @example /path/to/file:severity
     */
    void set_logger(std::string const &formatted_log_str);
    /**
     * Sets the logger to the specified formatter string
     * 
     * @param[in] file_path A path to the file to output the logs
     * @param[in] max_severity The max severity of logs to output
     * @throw std::invalid_argument if the logger could not be initialized due to incorrect arguments
     */
    void set_logger(std::filesystem::path const &file_path, LogSeverity const &max_severity);
} // namespace common::logging
