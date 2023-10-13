#include <atomic>
#include <chrono>
#include <thread>
#include <iostream>
#include <algorithm>
#include "jhLog.h"
#include "logMsg.h"

void jhlog_default_sink_example();
void jhlog_file_sink_example();
void jhlog_udp_sink_example();
void jhlog_user_sink_example();
void jhlog_multi_sink_example();
void jhlog_set_sinkLevel_example();
void jhlog_set_disabledModules_example();
void jhlog_thread_safe_example();
void jhlog_stdout_time_consuming();
void jhlog_file_time_consuming();
void jhlog_logMsg_test();

int main()
{
    /**You can open the comments to select which function you want to test*/
    // jhlog_default_sink_example();
    // jhlog_file_sink_example();
    // jhlog_udp_sink_example();
    // jhlog_user_sink_example();
    // jhlog_multi_sink_example();
    // jhlog_set_sinkLevel_example();
    // jhlog_set_disabledModules_example();
    // jhlog_thread_safe_example();
    // jhlog_stdout_time_consuming();
    // jhlog_file_time_consuming();
    // jhlog_logMsg_test();

    return 0;
}

void jhlog_default_sink_example()
{
    jhLog::log(jhLog::logLevel::TRACE, "jhlog_default_sink_example", JH_LOG_SOURCE, "hello world!!!");
    jhLog::log(jhLog::logLevel::DEBUG, JH_LOG_SOURCE, "jhlog will not output the message module");
    jhLog::log(jhLog::logLevel::INFO, "jhlog_default_sink_example", "jhlog will not output the file name, function name, line number, process id, thread id");
    jhLog::log(jhLog::logLevel::WARN, "jhlog will not output the file name, function name, line number, process id, thread id or message module");
    jhLog::log(jhLog::logLevel::ERROR, "jhlog will output parameter, eg. int: {}, double: {}, string: {}", 1, 2.0, "hello world!!!");
    
    jhLog::trace("jhlog will output trace level message directly");
    jhLog::debug("jhlog will output debug level message directly");
    jhLog::info("jhlog will output info level message directly");
    jhLog::warn("jhlog will output warn level message directly");
    jhLog::error("jhlog will output error level message directly");

    JH_LOG(jhLog::logLevel::TRACE, "jhlog will output trace level message with macro");
    JH_LOG(jhLog::logLevel::DEBUG, "jhlog will output debuf level message with macro");
    JH_LOG(jhLog::logLevel::INFO, "jhlog will output info level message with macro");
    JH_LOG(jhLog::logLevel::WARN, "jhlog will output warn level message with macro");
    JH_LOG(jhLog::logLevel::ERROR, "jhlog will output error level message with macro");

    
    JH_LOG_MOD(jhLog::logLevel::TRACE, "jhlog_default_sink_example", "jhlog will output trace level message and module tag with macro");
    JH_LOG_MOD(jhLog::logLevel::DEBUG, "jhlog_default_sink_example", "jhlog will output debug level message and module tag with macro");
    JH_LOG_MOD(jhLog::logLevel::INFO, "jhlog_default_sink_example", "jhlog will output info level message and module tag with macro");
    JH_LOG_MOD(jhLog::logLevel::WARN, "jhlog_default_sink_example", "jhlog will output warn level message and module tag with macro");
    JH_LOG_MOD(jhLog::logLevel::ERROR, "jhlog_default_sink_example", "jhlog will output error level message and module tag with macro");
}

void jhlog_file_sink_example()
{
    jhLog::PtrSink file_sink = std::make_shared<jhLog::simpleFileSink>("simpe_file_sink", "/workspaces/my_test/jhLog/logs");
    auto instance = jhLog::getInstance();
    //you can erase default sink or not. by your choice.If you erase default sink, jhlog will not output message to stdout.
    auto default_sink = instance->getSink(DEFAULT_SINK_NAME);
    instance->eraseSink(default_sink);
    instance->appendSink(file_sink);

    //then you can use jhlog as the default sink example.
    jhlog_default_sink_example();
}

void jhlog_udp_sink_example()
{
    jhLog::PtrSink udp_sink = std::make_shared<jhLog::udpSink>("udp_sink", "10.119.74.61", 20481);
    auto instance = jhLog::getInstance();
    instance->appendSink(udp_sink);
    
    //then you can use jhlog as the default sink example.
    jhlog_default_sink_example();
}

class userSink : public jhLog::baseSink
{
    public:
        userSink(const std::string& sink_name) : jhLog::baseSink(sink_name) {}
        virtual ~userSink() = default;
        userSink(const userSink&) = delete;
        userSink& operator=(const userSink&) = delete;

        virtual void log(const jhLog::logMsg& msg) override
        {
            std::cout << "userSink: " << msg.getMsg() << std::endl;
        }

        virtual void flush() override
        {
            //nothing to do
        }
};
void jhlog_user_sink_example()
{
    jhLog::PtrSink user_sink = std::make_shared<userSink>("user_sink");
    auto instance = jhLog::getInstance();
    instance->appendSink(user_sink);
    
    //then you can use jhlog as the default sink example.
    jhlog_default_sink_example();
}

void jhlog_multi_sink_example()
{
    auto instance = jhLog::getInstance();
    //set multi sinks
    std::vector<jhLog::PtrSink> sinks;
    sinks.push_back(std::make_shared<jhLog::simpleFileSink>("simpe_file_sink", "/workspaces/my_test/jhLog/logs"));
    sinks.push_back(std::make_shared<jhLog::udpSink>("udp_sink", "10.119.74.61", 20481));
    sinks.push_back(std::make_shared<jhLog::stdoutSink>("stdout_sink"));
    //setSinks will erase all sinks and set new sinks.
    instance->setSinks(sinks.begin(), sinks.end());

    //then you can use jhlog as the default sink example.
    jhlog_default_sink_example();
}

void jhlog_set_sinkLevel_example()
{
    auto instance = jhLog::getInstance();

    //set global level
    instance->setLevel(jhLog::logLevel::INFO);

    //set sink level
    auto file_sink = std::make_shared<jhLog::simpleFileSink>("simpe_file_sink", "/workspaces/my_test/jhLog/logs");
    file_sink->setSevel(jhLog::logLevel::DEBUG);

    auto stdout_sink = std::make_shared<jhLog::stdoutSink>("stdout_sink");
    stdout_sink->setSevel(jhLog::logLevel::WARN);

    instance->setSinks({file_sink, stdout_sink});

    jhLog::trace("The trace level message will not output");
    jhLog::debug("The debug level message will not output");
    jhLog::info("The info level message will output at file sink, but not output at stdout sink");
    jhLog::warn("The debug level message will output at file sink and stdout sink");
    jhLog::error("The debug level message will output at file sink and stdout sink");
}

void jhlog_set_disabledModules_example()
{
    auto instance = jhLog::getInstance();

    //set sink level
    std::vector<std::string> file_sink_disabled_modules = {"module1", "module2"};
    auto file_sink = std::make_shared<jhLog::simpleFileSink>("simpe_file_sink", "/workspaces/my_test/jhLog/logs");
    std::for_each(file_sink_disabled_modules.begin(), file_sink_disabled_modules.end(), [&file_sink](const std::string& module_name) {
        file_sink->appendDisMod(module_name);
    });

    std::vector<std::string> stdout_sink_disabled_modules = {"module2", "module3"};
    auto stdout_sink = std::make_shared<jhLog::stdoutSink>("stdout_sink");
    instance->setSinks({file_sink, stdout_sink});
    std::for_each(stdout_sink_disabled_modules.begin(), stdout_sink_disabled_modules.end(), [&instance](const std::string& module_name) {
        instance->appendDisMod("stdout_sink", module_name);
    });

    jhLog::trace("The messages without module tags are always output");
    jhLog::debug("module1", JH_NULL_SOURCE, "The message will NOT output at file sink, but output at stdout sink");
    jhLog::log(jhLog::logLevel::INFO, "module2", JH_NULL_SOURCE, "The message will NOT output at file sink or stdout sink");
    JH_WARN_MOD("module3", "The message will output at file sink, but NOT output at stdout sink")
}

// int count = 0;
std::atomic<int> count = 0;

void log_output()
{
    // jhLog::info(NULL_MODULE, JH_LOG_SOURCE, "count: {}", count++);
    jhLog::info("count: {}", count++);
}

void thread_func()
{
    for (int i = 0; i < 10000; ++i)
    {
        log_output();
    }
}

void jhlog_thread_safe_example()
{
    jhLog::PtrSink file_sink = std::make_shared<jhLog::simpleFileSink>("simpe_file_sink", "/workspaces/my_test/jhLog/logs");
    auto instance = jhLog::getInstance();
    instance->setSinks({file_sink});
    std::thread ths[10];
    auto milliseconds1 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    for (int i = 0; i < 10; ++i)
    {
        ths[i] = std::thread(thread_func);
    }

    for (int i = 0; i < 10; ++i)
    {
        ths[i].join();
    }
    auto milliseconds2 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    std::cout << "time: " << milliseconds2 - milliseconds1 << std::endl;
}

void jhlog_stdout_time_consuming()
{
    auto milliseconds1 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    for (int i = 0; i < 100000; ++i)
    {
        jhLog::info("int = {}, double = {}, string = {}, times = {}", 1, 2.0, "hello world!!!", i);
    }
    auto milliseconds2 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    std::cout << "time: " << milliseconds2 - milliseconds1 << std::endl;
}

void jhlog_file_time_consuming()
{
    jhLog::PtrSink file_sink = std::make_shared<jhLog::simpleFileSink>("simpe_file_sink", "/workspaces/my_test/jhLog/logs");
    auto instance = jhLog::getInstance();
    instance->setSinks({file_sink});
    auto milliseconds1 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    for (int i = 0; i < 100000; ++i)
    {
        jhLog::info("int = {}, double = {}, string = {}, times = {}", 1, 2.0, "hello world!!!", i);
    }
    auto milliseconds2 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    std::cout << "time: " << milliseconds2 - milliseconds1 << std::endl;
}

void jhlog_logMsg_test()
{
    jhLog::logMsg msg1("jhlog_logMsg_test", jhLog::logLevel::INFO, JH_LOG_SOURCE);
    msg1.toMsg("int = {}, double = {}, str = {}", 1, 2.0, "hello world!!!");
    std::cout << msg1.getMsg() << std::endl;
    auto msg1_copy1 = msg1;
    std::cout << msg1_copy1.getMsg() << std::endl;
    auto msg1_move1 = std::move(msg1);
    std::cout << msg1_move1.getMsg() << std::endl;
    std::cout << msg1.getMsg() << std::endl;
}