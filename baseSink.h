#pragma once

#include <mutex>
#include <atomic>
#include <string>
#include <unordered_set>
#include "logMsg.h"

namespace jhLog {

class baseSink
{
    public:
        baseSink(const std::string& sink_name) : m_sinkName(sink_name) {}
        virtual ~baseSink() = default;
        virtual void log(const logMsg& msg) = 0;
        virtual void flush() = 0;

        std::string sinkName() const
        {
            return m_sinkName;
        }

        void setSevel(logLevel log_level)
        {
            m_level.store(log_level);
        }

        logLevel level() const
        {
            return m_level.load();
        }

        void appendDisMod(const std::string& module_name)
        {
            std::lock_guard<std::mutex> lock(m_mutexModules);
            m_disableModules.insert(module_name);
        }

        void clearDisMod()
        {
            std::lock_guard<std::mutex> lock(m_mutexModules);
            m_disableModules.clear();
        }

        void eraseDisMod(const std::string& module_name)
        {
            std::lock_guard<std::mutex> lock(m_mutexModules);
            m_disableModules.erase(module_name);
        }

    protected:
        bool shouldLog(logLevel msg_level, const std::string& msg_moudle) const
        {
            std::lock_guard<std::mutex> lock(m_mutexModules);
            return msg_level >= m_level.load() && (msg_moudle == "" || m_disableModules.find(msg_moudle) == m_disableModules.end());
        }

    protected:
        std::string m_sinkName;
        std::atomic<logLevel> m_level{logLevel::TRACE};
        mutable std::mutex m_mutexModules;
        std::unordered_set<std::string> m_disableModules;
};

}