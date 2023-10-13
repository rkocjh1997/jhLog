#pragma once
#include <thread>
#include <vector>
#include <string>
#include "logMsg.h"
#include "baseSink.h"

#include <iostream>

namespace jhLog {

using PtrSink = std::shared_ptr<baseSink>;
using WPtrSink = std::weak_ptr<baseSink>;

class logger {
    public:
        logger(const logger &) = delete;
        logger &operator=(const logger &) = delete;

        static logger& instance()
        {
            static logger s_instance;
            return s_instance;
        }

        void setSinks(std::initializer_list<PtrSink> sinks)
        {
            setSinks(sinks.begin(), sinks.end());
        }

        template <typename It>
        void setSinks(It begin, It end)
        {
            std::lock_guard<std::mutex> lock(m_mutexSinks);
            m_sinks.clear();
            m_sinkMap.clear();
            for (auto it = begin; it != end; ++it) {
                m_sinks.push_back(*it);
                m_sinkMap.insert({(*it)->sinkName(), *it});
            }
        }
        void appendSink(PtrSink sink);

        void clearSinks();
        void eraseSink(PtrSink sink);

        std::vector<PtrSink> getSink();
        PtrSink getSink(const std::string& sink_name);

        void setLevel(logLevel log_level);
        logLevel level() const;

        void appendDisMod(const std::string& sink_name, const std::string& module_name);
        void clearDisMod();
        void eraseDisMod(const std::string& sink_name, const std::string& module_name);

        template<typename... Arg>
        void log(logLevel msg_level, const std::string& msg_module, logSource source, const std::string& fmt, Arg... arg)
        {
            if (shouldLog(msg_level)) {
                logMsg msg(msg_module, msg_level, source);
                msg.toMsg(fmt, arg...);
                auto sinks = getSink();
                for (auto& sink : sinks) {
                    sink->log(msg);
                }
            } 
        }

    private:
        logger();
        ~logger() {
            for (auto& sink : m_sinks) {
                sink->flush();
            }
        }
        bool shouldLog(logLevel msg_level) const;

    private:
        mutable std::mutex m_mutexSinks;
        std::vector<PtrSink> m_sinks;
        std::unordered_map<std::string, WPtrSink> m_sinkMap;
        std::atomic<logLevel> m_globalLevel{logLevel::TRACE};

        bool m_times[1000000];
};

} // namespace jhLog
