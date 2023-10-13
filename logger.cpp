#include "logger.h"
#include "stdoutSink.h"
namespace jhLog {

logger::logger()
{
    auto default_sink = std::make_shared<stdoutSink>(DEFAULT_SINK_NAME);
    m_sinks.push_back(default_sink);
    m_sinkMap.insert({default_sink->sinkName(), default_sink});
}

void logger::appendSink(PtrSink sink)
{
    std::lock_guard<std::mutex> lock(m_mutexSinks);
    m_sinks.push_back(sink);
    m_sinkMap.insert({sink->sinkName(), sink});
}

void logger::clearSinks()
{
    std::lock_guard<std::mutex> lock(m_mutexSinks);
    m_sinks.clear();
    m_sinkMap.clear();
}

void logger::eraseSink(PtrSink sink)
{
    std::lock_guard<std::mutex> lock(m_mutexSinks);
    auto it = std::find(m_sinks.begin(), m_sinks.end(), sink);
    if (it != m_sinks.end())
        m_sinks.erase(it);
    m_sinkMap.erase(sink->sinkName());
}

std::vector<PtrSink> logger::getSink()
{
    std::lock_guard<std::mutex> lock(m_mutexSinks);
    return m_sinks;
}

PtrSink logger::getSink(const std::string& sink_name)
{
    std::lock_guard<std::mutex> lock(m_mutexSinks);
    auto it = m_sinkMap.find(sink_name);
    if (it != m_sinkMap.end())
        return it->second.lock();
    return nullptr;
}

void logger::setLevel(logLevel log_level)
{
    m_globalLevel.store(log_level);
}

logLevel logger::level() const
{
    return m_globalLevel.load();
}

void logger::appendDisMod(const std::string& sink_name, const std::string& module_name)
{
    auto sink = getSink(sink_name);
    if (sink)
        sink->appendDisMod(module_name);
}

void logger::clearDisMod()
{
    auto sinks = getSink();
    for (auto& sink : sinks)
        sink->clearDisMod();
}

void logger::eraseDisMod(const std::string& sink_name, const std::string& module_name)
{
    auto sink = getSink(sink_name);
    if (sink)
        sink->eraseDisMod(module_name);
}

bool logger::shouldLog(logLevel msg_level) const
{
    return msg_level >= m_globalLevel.load();
}
}