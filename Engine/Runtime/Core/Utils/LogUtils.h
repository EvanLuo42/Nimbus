#pragma once

#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

inline std::shared_ptr<spdlog::logger> GetLogger(const std::string& module) {
    auto logger = spdlog::get(module);
    if (!logger) {
        logger = spdlog::stdout_color_mt(module);
        // logger->set_level(spdlog::level::debug);
        logger->set_pattern("[%H:%M:%S] [%^%5l%$] [%n] [%s:%#] %v");
    }
    return logger;
}

#define NB_DEBUG(fmt, ...) SPDLOG_LOGGER_DEBUG(Logger, fmt, ##__VA_ARGS__)
#define NB_INFO(fmt, ...) SPDLOG_LOGGER_INFO(Logger, fmt, ##__VA_ARGS__)
#define NB_WARN(fmt, ...) SPDLOG_LOGGER_WARN(Logger, fmt, ##__VA_ARGS__)
#define NB_ERROR(fmt, ...) SPDLOG_LOGGER_ERROR(Logger, fmt, ##__VA_ARGS__)
