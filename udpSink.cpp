#include <sys/socket.h>
#include <arpa/inet.h>
#include "udpSink.h"

namespace jhLog {
udpSink::udpSink(const std::string& sink_name, const std::string& ip, uint16_t port) :
    baseSink(sink_name)
{
    memset(&m_distAddr, 0, sizeof(m_distAddr));
    m_distAddr.sin_family = AF_INET;
    m_distAddr.sin_port = htons(port);
    m_distAddr.sin_addr.s_addr = inet_addr(ip.c_str());
    m_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (m_sockfd < 0) {
        std::runtime_error("socket error, errno: " + std::to_string(errno) + ", errmsg: " + strerror(errno));
    }

}

udpSink::~udpSink()
{
    if (m_sockfd > -1)
        close(m_sockfd);
}

void udpSink::log(const logMsg& msg)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!shouldLog(msg.getLevel(), std::string(msg.getModName()))) {
        return;
    }
    sendto(m_sockfd, msg.getMsg().data(), msg.getMsgSize(), 0, (struct sockaddr*)&m_distAddr, sizeof(m_distAddr));
}
}