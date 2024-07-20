#include "Logger.h"

std::shared_ptr<spdlog::logger> Logger::logger;

void Logger::init() {
    logger = spdlog::basic_logger_mt("basic_logger", "logs/logfile.log");
    spdlog::set_level(spdlog::level::debug);
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%l] %v");
}

std::shared_ptr<spdlog::logger>& Logger::getLogger() {
    return logger;
}
