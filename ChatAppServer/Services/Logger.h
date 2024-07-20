#ifndef LOGGER_H
#define LOGGER_H

#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

class Logger {
public:
    static void init();
    static std::shared_ptr<spdlog::logger>& getLogger();

private:
    static std::shared_ptr<spdlog::logger> logger;
};

#define LOG_INFO(...) Logger::getLogger()->info(__VA_ARGS__)
#define LOG_WARN(...) Logger::getLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) Logger::getLogger()->error(__VA_ARGS__)
#define LOG_DEBUG(...) Logger::getLogger()->debug(__VA_ARGS__)

#endif // LOGGER_H