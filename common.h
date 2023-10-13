#pragma once
#include <string_view>
#include <unordered_map>

static const char* DEFAULT_SINK_NAME =  "JHLOG_DEFAULT_SINK";

namespace jhLog {

class fileHelper {
    public:
        explicit fileHelper(const std::string& filename)
        {
            m_file = fopen(filename.c_str(), "a");
            if (m_file == nullptr) {
                throw std::runtime_error("Failed to open file " + filename);
            }
        }
        void write(std::string_view msg)
        {
            fwrite(msg.data(), msg.size(), sizeof(char), m_file);
        }
        void flush()
        {
            fflush(m_file);
        }
        virtual ~fileHelper() 
        {
            if (m_file != nullptr) {
                fclose(m_file);
            }
        }
        fileHelper(const fileHelper&) = delete;
        fileHelper& operator=(const fileHelper&) = delete;

    private:
        FILE* m_file;
        std::string m_filename;
};

enum class logLevel : int
{
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR
};

static std::unordered_map<logLevel, std::string> log_level_str =
{
    {logLevel::TRACE, "TRACE"},
    {logLevel::DEBUG, "DEBUG"},
    {logLevel::INFO, "INFO"},
    {logLevel::WARN, "WARN"},
    {logLevel::ERROR, "ERROR"}
};
}