#include "logger.h"
#include "common.h"
#include "udpSink.h"
#include "baseSink.h"
#include "stdoutSink.h"
#include "simpleFileSink.h"

namespace jhLog {

inline logger* getInstance()
{
    return &logger::instance();
}

template<typename... Arg>
inline void log(logLevel msg_level, const std::string& msg_module, logSource source, const std::string& fmt, Arg... arg)
{
    logger::instance().log(msg_level, msg_module, source, fmt, arg...);
}

template<typename... Arg>
inline void log(logLevel msg_level, const std::string& msg_module, const std::string& fmt, Arg... arg)
{
    logSource source{};
    logger::instance().log(msg_level, msg_module, source, fmt, arg...);
}

template<typename... Arg>
inline void log(logLevel msg_level, logSource source, const std::string& fmt, Arg... arg)
{
    logger::instance().log(msg_level, "", source, fmt, arg...);
}

template <typename... Args>
inline void log(logLevel msg_level, const std::string&  fmt, Args... args)
{
    logSource source{};
    logger::instance().log(msg_level, "", source, fmt, args...);
}

template <typename... Args>
inline void trace(const std::string& msg_module, logSource source, const std::string& fmt, Args... args)
{
    log(logLevel::TRACE, msg_module, source, fmt, args...);
}

template <typename... Args>
inline void trace(const std::string& fmt, Args... args)
{
    logSource source{};
    log(logLevel::TRACE, "", source, fmt, args...);
}

template <typename... Args>
inline void debug(const std::string& msg_module, logSource source, const std::string& fmt, Args... args)
{
    log(logLevel::DEBUG, msg_module, source, fmt, args...);
}

template <typename... Args>
inline void debug(const std::string& fmt, Args... args)
{
    logSource source{};
    log(logLevel::DEBUG, "", source, fmt, args...);
}

template <typename... Args>
inline void info(const std::string& msg_module, logSource source, const std::string& fmt, Args... args)
{
    log(logLevel::INFO, msg_module, source, fmt, args...);
}

template <typename... Args>
inline void info(const std::string& fmt, Args... args)
{
    logSource source{};
    log(logLevel::INFO, "", source, fmt, args...);
}

template <typename... Args>
inline void warn(const std::string& msg_module, logSource source, const std::string& fmt, Args... args)
{
    log(logLevel::WARN, msg_module, source, fmt, args...);
}

template <typename... Args>
inline void warn(const std::string& fmt, Args... args)
{
    logSource source{};
    log(logLevel::WARN, "", source, fmt, args...);
}

template <typename... Args>
inline void error(const std::string& msg_module, logSource source, const std::string& fmt, Args... args)
{
    log(logLevel::ERROR, msg_module, source, fmt, args...);
}

template <typename... Args>
inline void error(const std::string& fmt, Args... args)
{
    logSource source{};
    log(logLevel::ERROR, "", source, fmt, args...);
}

}

// #define JH_LOG_SOURCE jhLog::logSource(__FILE__, __FUNCTION__, __LINE__)
#define JH_LOG_SOURCE {__FILE__, __FUNCTION__, __LINE__}
#define JH_NULL_SOURCE {nullptr, nullptr, 0}
#define NULL_MODULE ""

#define JH_LOG(msg_level, fmt, ...) jhLog::log(msg_level, NULL_MODULE, JH_LOG_SOURCE, fmt, ##__VA_ARGS__);
#define JH_LOG_MOD(msg_level, msg_module, fmt, ...) jhLog::log(msg_level, msg_module, JH_LOG_SOURCE, fmt, ##__VA_ARGS__);

#define JH_TRACE(fmt, ...) jhLog::trace(NULL_MODULE, JH_LOG_SOURCE, fmt, ##__VA_ARGS__);
#define JH_TRACE_MOD(msg_module, fmt, ...) jhLog::trace(msg_module, JH_LOG_SOURCE, fmt, ##__VA_ARGS__);

#define JH_DEBUG(fmt, ...) jhLog::debug(NULL_MODULE, JH_LOG_SOURCE, fmt, ##__VA_ARGS__);
#define JH_DEBUG_MOD(msg_module, fmt, ...) jhLog::debug(msg_module, JH_LOG_SOURCE, fmt, ##__VA_ARGS__);

#define JH_INFO(fmt, ...) jhLog::info(NULL_MODULE, JH_LOG_SOURCE, fmt, ##__VA_ARGS__);
#define JH_INFO_MOD(msg_module, fmt, ...) jhLog::info(msg_module, JH_LOG_SOURCE, fmt, ##__VA_ARGS__);

#define JH_WARN(fmt, ...) warn(NULL_MODULE, JH_LOG_SOURCE, fmt, ##__VA_ARGS__);
#define JH_WARN_MOD(msg_module, fmt, ...) jhLog::warn(msg_module, JH_LOG_SOURCE, fmt, ##__VA_ARGS__);

#define JH_ERROR(fmt, ...) jhLog::error(NULL_MODULE, JH_LOG_SOURCE, fmt, ##__VA_ARGS__);
#define JH_ERROR_MOD(msg_module, fmt, ...) jhLog::error(msg_module, JH_LOG_SOURCE, fmt, ##__VA_ARGS__);