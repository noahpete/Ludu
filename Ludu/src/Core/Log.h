#pragma once

#include "Core/Core.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Ludu {

    // from https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Hazel/Core/Log.h

    class Log
    {
    public:
        static void Init();
        static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

    private:
        static Ref<spdlog::logger> s_CoreLogger;
        static Ref<spdlog::logger> s_ClientLogger;

    };
}

#define LD_CORE_TRACE(...)      ::Ludu::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define LD_CORE_INFO(...)       ::Ludu::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LD_CORE_WARN(...)       ::Ludu::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LD_CORE_ERROR(...)      ::Ludu::Log::GetCoreLogger()->error(__VA_ARGS__)
#define LD_CORE_CRITICAL(...)   ::Ludu::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define LD_TRACE(...)      ::Ludu::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LD_INFO(...)       ::Ludu::Log::GetClientLogger()->info(__VA_ARGS__)
#define LD_WARN(...)       ::Ludu::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LD_ERROR(...)      ::Ludu::Log::GetClientLogger()->error(__VA_ARGS__)
#define LD_CRITICAL(...)   ::Ludu::Log::GetClientLogger()->critical(__VA_ARGS__)