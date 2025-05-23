#pragma once

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG

#include <memory>
#include <print>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace NimbusEngine
{
enum class LoggerType
{
    Client,
    Engine
};

static std::vector<spdlog::sink_ptr> logSinks = {std::make_shared<spdlog::sinks::stdout_color_sink_mt>()};
}

inline std::shared_ptr<spdlog::logger> GetLogger(const NimbusEngine::LoggerType type) {
    switch (type)
    {
    case NimbusEngine::LoggerType::Engine: {
        if (auto logger = spdlog::get("Engine"); !logger)
        {
            logger = std::make_shared<spdlog::logger>("Engine", std::begin(NimbusEngine::logSinks), std::end(NimbusEngine::logSinks));
            logger->set_pattern("[%H:%M:%S] [%^%5l%$] [%n] [%s:%#] %v");
            logger->set_level(spdlog::level::trace);
            return logger;
        }
    }
    case NimbusEngine::LoggerType::Client: {
        if (auto logger = spdlog::get("Client"); !logger)
        {
            logger = std::make_shared<spdlog::logger>("Client", std::begin(NimbusEngine::logSinks), std::end(NimbusEngine::logSinks));
            logger->set_pattern("[%H:%M:%S] [%^%5l%$] [%n] [%s:%#] %v");
            logger->set_level(spdlog::level::trace);
            return logger;
        }
    }
    default:
        throw std::runtime_error("Unknown logger type");
    }
}

#define NB_DEBUG(fmt, ...) SPDLOG_LOGGER_DEBUG(GetLogger(NimbusEngine::LoggerType::Client), fmt, ##__VA_ARGS__)
#define NB_INFO(fmt, ...) SPDLOG_LOGGER_INFO(GetLogger(NimbusEngine::LoggerType::Client), fmt, ##__VA_ARGS__)
#define NB_WARN(fmt, ...) SPDLOG_LOGGER_WARN(GetLogger(NimbusEngine::LoggerType::Client), fmt, ##__VA_ARGS__)
#define NB_ERROR(fmt, ...) SPDLOG_LOGGER_ERROR(GetLogger(NimbusEngine::LoggerType::Client), fmt, ##__VA_ARGS__)

#define NB_CORE_DEBUG(fmt, ...) SPDLOG_LOGGER_DEBUG(GetLogger(NimbusEngine::LoggerType::Engine), fmt, ##__VA_ARGS__)
#define NB_CORE_INFO(fmt, ...) SPDLOG_LOGGER_INFO(GetLogger(NimbusEngine::LoggerType::Engine), fmt, ##__VA_ARGS__)
#define NB_CORE_WARN(fmt, ...) SPDLOG_LOGGER_WARN(GetLogger(NimbusEngine::LoggerType::Engine), fmt, ##__VA_ARGS__)
#define NB_CORE_ERROR(fmt, ...) SPDLOG_LOGGER_ERROR(GetLogger(NimbusEngine::LoggerType::Engine), fmt, ##__VA_ARGS__)
