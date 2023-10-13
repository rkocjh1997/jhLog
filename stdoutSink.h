#pragma once

#include "baseSink.h"

namespace jhLog {

class stdoutSink final : public baseSink
{
    public:
        stdoutSink(const std::string& sink_name);
        ~stdoutSink() override = default;
        void log(const logMsg& msg) override;
        void flush() override;

    private:
        std::mutex m_mutex;
};

}