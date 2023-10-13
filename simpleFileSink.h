#pragma once

#include <memory>
#include "baseSink.h"

namespace jhLog {

class simpleFileSink final : public baseSink
{
    public:
        simpleFileSink(const std::string& sink_name, const std::string& path);
        ~simpleFileSink() override = default;
        void log(const logMsg& msg) override;
        void flush() override;

    private:
        std::mutex m_mutex;
        std::unique_ptr<fileHelper> m_pFileHelper;
};

}