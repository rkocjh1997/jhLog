#pragma once

#include <string>
#include <string_view>
#include <unistd.h>
#include <sys/time.h>
#include <sys/syscall.h>
#define FMT_HEADER_ONLY
#include "fmt/format.h"
#include "common.h"
namespace jhLog {

struct logSource
{
    const char* file_name;
    const char* func_name;
    int line;
    int pid;
    int tid;
    logSource() = default;
    logSource(const char* file_name, const char* func_name, int line) : 
        file_name(file_name == nullptr ? file_name : basename(file_name)), 
        func_name(func_name), 
        line(line)
    {
        if (file_name == nullptr || func_name == nullptr)
        {
            pid = getpid();
            tid = syscall(SYS_gettid);
        }
    }
};

struct logMsg
{
    std::string_view moudle_name;
    logLevel level;

    char* header;
    std::string msg;

    logMsg(std::string_view _moudle_name, logLevel _level, logSource _source) :
        moudle_name(_moudle_name),
        level(_level)
    {
        size_t size = 256 + moudle_name.size();
        if (_source.file_name != nullptr && _source.func_name != nullptr)
            size += strlen(_source.file_name) + strlen(_source.func_name);
        header = new char[size];
        struct timeval tv;
        gettimeofday(&tv, nullptr);
        char time_str[64];
        strftime(time_str, size, "%Y-%m-%d %H:%M:%S", std::localtime(&tv.tv_sec));
        snprintf(header, size, "[%s.%03ld] [%s] ", time_str, tv.tv_usec / 1000, log_level_str[level].c_str());

        if (!moudle_name.empty())
            snprintf(header + strlen(header), size - strlen(header), "[%s] ", moudle_name.data());
        if (_source.file_name != nullptr && _source.func_name != nullptr)
        {
            snprintf(header + strlen(header), size - strlen(header), "[%s:%d:%s] [(pid)%d:(tid)%d] ", _source.file_name, _source.line, _source.func_name, _source.pid, _source.tid);
        }
        msg.append(header);
    }
    logMsg(std::string_view _moudle_name, logLevel _level) :
        logMsg(_moudle_name, _level, logSource(nullptr, nullptr, 0)) {}

    ~logMsg()
    {
        delete[] header;
    }

    logMsg(const logMsg& other)
    {
        moudle_name = other.moudle_name;
        level = other.level;
        header = new char[strlen(other.header) + 1];
        strcpy(header, other.header);
        msg = other.msg;
    } 
    logMsg& operator=(const logMsg& other)
    {
        if (this == &other)
            return *this;
        delete[] header;
        moudle_name = other.moudle_name;
        level = other.level;
        header = new char[strlen(other.header) + 1];
        strcpy(header, other.header);
        msg = other.msg;
        return *this;
    }

    logMsg(logMsg&& other) noexcept
    {
        moudle_name = other.moudle_name;
        level = other.level;
        header = other.header;
        msg = std::move(other.msg);
        other.header = nullptr;
    }
    logMsg& operator=(logMsg&& other) noexcept
    {
        if (this == &other)
            return *this;
        delete[] header;
        moudle_name = other.moudle_name;
        level = other.level;
        header = other.header;
        msg = std::move(other.msg);
        other.header = nullptr;
        return *this;
    }

    template<typename... Args>
    void toMsg(std::string_view _format, Args&&... args)
    {
        msg.append(fmt::format(_format, std::forward<Args>(args)...));
        msg.push_back('\n');
    }

    std::string_view getMsg() const
    {
        return msg;
    }

    size_t getMsgSize() const
    {
        return msg.size();
    }

    logLevel getLevel() const
    {
        return level;
    }

    std::string_view getModName() const
    {
        return moudle_name;
    }
};

}