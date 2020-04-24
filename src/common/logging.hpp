#pragma once

#include <boost/log/trivial.hpp>
#include <string>
#include <any>
#include <filesystem>
#include <iostream>
#include <typeinfo>

#ifdef NDEBUG
#define LOG_DEBUG common::logging::LogEntry(common::logging::LogSeverity::DEBUG)
#define LOG_INFO common::logging::LogEntry(common::logging::LogSeverity::INFO)
#define LOG_WARNING common::logging::LogEntry(common::logging::LogSeverity::WARNING)
#define LOG_ERROR common::logging::LogEntry(common::logging::LogSeverity::ERROR)
#else
#define LOG_DEBUG common::logging::LogEntry(common::logging::LogSeverity::DEBUG, __LINE__, __func__)
#define LOG_INFO common::logging::LogEntry(common::logging::LogSeverity::INFO, __LINE__, __func__)
#define LOG_WARNING common::logging::LogEntry(common::logging::LogSeverity::WARNING, __LINE__, __func__)
#define LOG_ERROR common::logging::LogEntry(common::logging::LogSeverity::ERROR, __LINE__, __func__)
#endif

namespace common::logging
{
enum class LogSeverity
{
    NOT_SET = -1,
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

// Wrapper class to create entries for the boost logger
typedef std::ostream &(*ostream_function)(std::ostream &);
class LogEntry
{
public:
#ifdef NDEBUG
    LogEntry(LogSeverity const &severity);
#else
    LogEntry(LogSeverity const &severity, long const &line, char const *const &func);
#endif
    ~LogEntry();
    // Used for logging different types
    LogEntry &operator<<(std::string const &str);
    LogEntry &operator<<(char const &c);
    LogEntry &operator<<(int const &num);
    LogEntry &operator<<(double const &num);
    LogEntry &operator<<(float const &num);
    LogEntry &operator<<(LogSeverity const &severity);
    // Used for things like std::endl for logging
    LogEntry &operator<<(ostream_function const &func);

private:
    void log_entry_();

    LogSeverity const severity_;
    std::string entry_;
};

void init_logger();
bool logger_initialized() noexcept;
void set_logger(std::string const &formatted_log_str);
void set_logger(std::filesystem::path const &file_path, LogSeverity const &max_severity);
} // namespace common::logging
