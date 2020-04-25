#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/attributes/mutable_constant.hpp>

#include "logging.hpp"

#ifdef NDEBUG
#define LOGGING_FORMAT "[%TimeStamp%] %LogSeverity% - %Message%"
#else
#define LOGGING_FORMAT "[%TimeStamp%:%Function%:%Line%] %LogSeverity% - %Message%"
#endif

namespace common::logging
{
namespace
{
bool initialized = false;

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

void parse_string_(std::string const &logging_str, std::filesystem::path *file_path,
                   std::string *file_name, LogSeverity *log_severity)
{
    // This should never happen unless the developer makes a mistake
    if (file_path == nullptr || file_name == nullptr || log_severity == nullptr)
    {
        throw std::runtime_error{"At least one logging parameter must not be null."};
    }

    size_t delimiter_index = logging_str.find(":");
    size_t file_name_start = logging_str.rfind("/");
    if (delimiter_index == std::string::npos ||
        (file_name_start != std::string::npos && file_name_start > delimiter_index))
    {
        throw std::runtime_error{"Logging string formatted incorrectly."};
    }
    // First index is the logging file, second index is the severity
    if (file_name_start != std::string::npos)
    {
        *file_path = std::filesystem::path{logging_str.substr(0, file_name_start + 1)};
        *file_name = logging_str.substr(file_name_start + 1, (delimiter_index - file_name_start - 1));
    }
    std::string log_severity_str = logging_str.substr(delimiter_index + 1, logging_str.size());
    std::transform(log_severity_str.begin(), log_severity_str.end(),
                   log_severity_str.begin(), [](unsigned char c) { return std::tolower(c); });

    LOG_DEBUG << "[LOGGING_STR] = " << logging_str
              << "; [FILE_PATH] = " << *file_path
              << "; [FILE_NAME] = " << *file_name
              << "; [SEVERITY_STR] = " << log_severity_str;

    *log_severity = string_to_log_severity_(log_severity_str);
}

// Set value for dynamically changing thread attributes
template <typename T>
void set_logging_thread_attribute_(std::string const &name, T const &value)
{
    boost::log::attribute_set attr_set =
        boost::log::core::get()->get_thread_attributes();

    // Check to see if the value exists in the set
    if (!attr_set.count(name))
    {
        boost::log::core::get()->add_thread_attribute(name, boost::log::attributes::mutable_constant<T>(value));
    }
    else
    {
        boost::log::attributes::mutable_constant<T> attr =
            boost::log::attribute_cast<boost::log::attributes::mutable_constant<T>>(attr_set[name]);
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
    if (!entry_.empty())
    {
        log_entry_();
    }
}

void LogEntry::log_entry_()
{
    // Check to see that the logger is initialized
    if (!logging_initialized())
    {
        throw std::runtime_error{"Logger has not been initialized!"};
    }

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
    log_severity_to_string_(log_severity);
    return *this;
}

LogEntry &LogEntry::operator<<(ostream_function const &func)
{
    log_entry_();
    return *this;
}

void init_logging()
{
    if (!initialized)
    {
        // Remove the current logging source
        boost::log::add_common_attributes();
        boost::log::add_console_log(std::cout, boost::log::keywords::format = LOGGING_FORMAT);
#ifdef NDEBUG
        // NOTE: Defaulting to error for release builds in case the user wants to continue using error as log severity level
        boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::error);
#else
        boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::debug);
#endif
        initialized = true;
    }
    else
    {
        LOG_DEBUG << "Unexpected call to init_logger -> Logging has already been initialized once... Continuing";
    }
    LOG_DEBUG << "Logging initialized successfully";
}

bool logging_initialized() noexcept
{
    return initialized;
}

void add_logger(std::filesystem::path const &file_path, std::string const &file_name, LogSeverity const &max_severity)
{
    if (!initialized)
    {
        throw std::runtime_error{"Logging has not been initialized."};
    }
    else if (max_severity == LogSeverity::UNKNOWN)
    {
        throw std::runtime_error{"Logging severity is not set."};
    }

    std::string const file_path_str = file_path.empty() ? "console" : file_path.u8string() + file_name;
    LOG_DEBUG << "Switching to logging at file '" << file_path_str << "' with severity '" << max_severity << "'";

    boost::log::trivial::severity_level boost_severity;
    switch (max_severity)
    {
    case LogSeverity::DEBUG:
        boost_severity = boost::log::trivial::debug;
        break;
    case LogSeverity::INFO:
        boost_severity = boost::log::trivial::info;
        break;
    case LogSeverity::WARNING:
        boost_severity = boost::log::trivial::warning;
        break;
    case LogSeverity::ERROR:
        boost_severity = boost::log::trivial::error;
        break;
    default:
        throw std::runtime_error{"Logging severity not supported."};
    }

#ifdef NDEBUG
    // Remove all current logging sources
    boost::log::core::get()->remove_all_sinks();
    boost::log::add_common_attributes();
    if (file_path.empty())
    {
        boost::log::add_console_log(std::cout, boost::log::keywords::format = LOGGING_FORMAT);
    }
    else
    {
        std::filesystem::create_directories(file_path);
        boost::log::add_file_log(file_path.u8string() + file_name, boost::log::keywords::format = LOGGING_FORMAT);
    }
    boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost_severity);
#else
    LOG_DEBUG << "Using DEBUG build -> Bypass setting the logger!";
#endif
}

void add_logger(std::string const &formatted_log_str)
{
    std::filesystem::path path;
    std::string file_name;
    LogSeverity severity = LogSeverity::UNKNOWN;
    parse_string_(formatted_log_str, &path, &file_name, &severity);
    add_logger(path, file_name, severity);
}
} // namespace common::logging