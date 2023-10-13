#include <chrono>
#include "simpleFileSink.h"

namespace jhLog {

simpleFileSink::simpleFileSink(const std::string& sink_name, const std::string& path) : 
    baseSink(sink_name)
{
    auto now = std::chrono::system_clock::now();
    std::time_t ts = std::chrono::system_clock::to_time_t(now);
    char time_str[32];
    strftime(time_str, sizeof(time_str), "%Y_%m_%d_%H_%M_%S", std::localtime(&ts));
    std::string filename = path + "/" + m_sinkName + "_" + time_str + ".log";
    m_pFileHelper = std::make_unique<fileHelper>(filename);
}

void simpleFileSink::log(const logMsg& msg)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!shouldLog(msg.getLevel(), std::string(msg.getModName()))) {
        return;
    }
    m_pFileHelper->write(msg.getMsg());
}

void simpleFileSink::flush()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_pFileHelper->flush();
}

}