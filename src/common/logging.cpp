#include <boost/log/core.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/attributes/mutable_constant.hpp>

#include "logging.hpp"

namespace common::logging
{
namespace
{
bool initialized = false;
void parse_string_(std::string const &logging_str, std::filesystem::path *file_path,
                   std::string *file_name, LogSeverity *logging_severity)
{
    // This should never happen unless the developer makes a mistake
    if (file_path == nullptr || file_name == nullptr || logging_severity == nullptr)
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
    std::string severity_str = logging_str.substr(delimiter_index + 1, logging_str.size());
    std::transform(severity_str.begin(), severity_str.end(),
                   severity_str.begin(), [](unsigned char c) { return std::tolower(c); });

    LOG_DEBUG << "[LOGGING_STR] = " << logging_str
              << "; [FILE_PATH] = " << *file_path
              << "; [FILE_NAME] = " << *file_name
              << "; [SEVERITY_STR] = " << severity_str;

    if (severity_str == "debug")
    {
        *logging_severity = LogSeverity::DEBUG;
    }
    else if (severity_str == "info")
    {
        *logging_severity = LogSeverity::INFO;
    }
    else if (severity_str == "warning")
    {
        *logging_severity = LogSeverity::WARNING;
    }
    else if (severity_str == "error")
    {
        *logging_severity = LogSeverity::ERROR;
    }
    else
    {
        LOG_WARNING << "Unable to parse severity '" << severity_str << "'... Defaulting to using " << LogSeverity::INFO << ".";
        *logging_severity = LogSeverity::INFO;
    }
}

// Set value for dynamically changing thread attributes
template <typename T>
void set_logging_attribute_value_(const char *name, T const value)
{
    boost::log::attributes::mutable_constant<T> attr =
        boost::log::attribute_cast<boost::log::attributes::mutable_constant<T>>(
            boost::log::core::get()->get_thread_attributes()[name]);
    attr.set(value);
}
} // namespace

#ifdef NDEBUG
LogEntry::LogEntry(LogSeverity const &severity)
    : severity_{severity}, entry_{""}
{
}
#else
LogEntry::LogEntry(LogSeverity const &severity, long const &line, char const *const &func)
    : severity_{severity}, entry_{""}
{
    set_logging_attribute_value_("Line", line);
    set_logging_attribute_value_("Function", func);
}
#endif

LogEntry::~LogEntry()
{
    log_entry_();
}

void LogEntry::log_entry_()
{
    // Check to see that the logger is initialized
    if (!logger_initialized())
    {
        throw std::runtime_error{"Logger has not been initialized!"};
    }

    switch (severity_)
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

LogEntry &LogEntry::operator<<(LogSeverity const &severity)
{
    switch (severity)
    {
    case LogSeverity::DEBUG:
        entry_.append("DEBUG");
        break;
    case LogSeverity::INFO:
        entry_.append("INFO");
        break;
    case LogSeverity::WARNING:
        entry_.append("WARNING");
        break;
    case LogSeverity::ERROR:
        entry_.append("ERROR");
        break;
    default:
        entry_.append("NOT_SET");
    }
    return *this;
}

LogEntry &LogEntry::operator<<(ostream_function const &func)
{
    log_entry_();
    return *this;
}

void init_logger()
{
    if (!initialized)
    {
        // Remove all current logging sources
        boost::log::core::get()->remove_all_sinks();
        boost::log::add_common_attributes();
#ifdef NDEBUG
        boost::log::add_console_log(std::cout, boost::log::keywords::format = "[%TimeStamp%] %Message%");
        boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::info);
// Sets the logger to debug mode for the console if running a debug build
#else
        // If debug build, log the line and function
        boost::log::core::get()->add_global_attribute("Line", boost::log::attributes::mutable_constant<long>(__LINE__));
        boost::log::core::get()->add_global_attribute("Function", boost::log::attributes::mutable_constant<std::string>(""));

        boost::log::add_console_log(std::cout, boost::log::keywords::format = "[%TimeStamp%:%Line%:%Function%] %Message%");
        boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::debug);
#endif
        initialized = true;
    }
    else
    {
        throw std::runtime_error{"Logging has already been initialized."};
    }
}

bool logger_initialized() noexcept
{
    return initialized;
}

void set_logger(std::filesystem::path const &file_path, std::string const &file_name, LogSeverity const &max_severity)
{
    if (!initialized)
    {
        throw std::runtime_error{"Logging has not been initialized."};
    }
    else if (max_severity == LogSeverity::NOT_SET)
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
        boost::log::add_console_log(std::cout, boost::log::keywords::format = "[%TimeStamp%] %Message%");
    }
    else
    {
        std::filesystem::create_directories(file_path);
        boost::log::add_file_log(file_path.u8string() + file_name, boost::log::keywords::format = "[%TimeStamp%] %Message%");
    }
    boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost_severity);
#else
    LOG_DEBUG << "Using DEBUG build -> Bypass setting the logger!";
#endif
}

void set_logger(std::string const &formatted_log_str)
{
    std::filesystem::path path;
    std::string file_name;
    LogSeverity severity = LogSeverity::NOT_SET;
    parse_string_(formatted_log_str, &path, &file_name, &severity);
    set_logger(path, file_name, severity);
}

namespace
{

} // namespace
} // namespace common::logging