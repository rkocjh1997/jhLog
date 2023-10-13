#pragma once

#include <netinet/in.h>
#include "baseSink.h"

namespace jhLog {

class udpSink final : public baseSink
{
    public:
        udpSink(const std::string& sink_name, const std::string& ip, uint16_t port);
        ~udpSink() override;
        void log(const logMsg& msg) override;
        void flush() override {}
    private:
        std::mutex m_mutex;
        int m_sockfd;
        struct sockaddr_in m_distAddr;

};

}