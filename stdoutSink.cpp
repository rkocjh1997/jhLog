#include <cstdio>
#include "stdoutSink.h"

namespace jhLog {

stdoutSink::stdoutSink(const std::string& sink_name) : baseSink(std::move(sink_name)) {}

void stdoutSink::log(const logMsg& msg) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!shouldLog(msg.getLevel(), std::string(msg.getModName()))) {
        return;
    }
    
    fwrite(msg.getMsg().data(), msg.getMsgSize(), sizeof(char), stdout);
    fflush(stdout);
}

void stdoutSink::flush() {
    std::lock_guard<std::mutex> lock(m_mutex);
    fflush(stdout);
}

}