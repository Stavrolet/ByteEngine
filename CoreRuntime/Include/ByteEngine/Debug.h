#pragma once

#define NOMINMAX

#include "ByteEngine/Core/Base/Application.h"
#include "ByteEngine/Core/Base/Singleton.h"

#include <quill/LogMacros.h>
#include <quill/Logger.h>
#include <source_location>
#include <stacktrace>

namespace ByteEngine
{
    class Debug : public Singleton<Debug>
    {
        friend class Application;

    private:
        quill::Logger* logger = nullptr;

    public:
        ~Debug() override { Shutdown(); }

        static void Breakpoint()
        {
#ifdef _MSC_VER
            __debugbreak();
#elif defined(__clang__)
            __builtin_debugtrap();
#elif defined(__GNUC__)
    #ifdef __x86_64__
            asm volatile("int $3 \n nop");
    #else
        #error "ByteEngine does not support current architecture"
    #endif
#else
    #error "ByteEngine does not support current compiler"
#endif
        }

        static void BreakpointIfDebuggerAttached();
        static bool IsDebuggerAttached();

        void* GetLogger() const
        {
            return logger;
        }

    private:
        void Initialize();
        void Shutdown();
    };
} // namespace ByteEngine

#define BE_LOG_INFO(fmt, ...) QUILL_LOG_INFO(static_cast<::quill::Logger*>(::ByteEngine::Debug::GetInstance().GetLogger()), fmt, ##__VA_ARGS__)
#define BE_LOG_WARNING(fmt, ...) QUILL_LOG_WARNING(static_cast<::quill::Logger*>(::ByteEngine::Debug::GetInstance().GetLogger()), fmt, ##__VA_ARGS__)
#define BE_LOG_ERROR(fmt, ...) QUILL_LOG_ERROR(static_cast<::quill::Logger*>(::ByteEngine::Debug::GetInstance().GetLogger()), fmt, ##__VA_ARGS__)
#define BE_LOG_CRITICAL(fmt, ...) QUILL_LOG_CRITICAL(static_cast<::quill::Logger*>(::ByteEngine::Debug::GetInstance().GetLogger()), fmt, ##__VA_ARGS__)

#define BE_CHECK(condition)                                                                       \
    do                                                                                            \
    {                                                                                             \
        if (!(condition)) [[unlikely]]                                                            \
        {                                                                                         \
            ::ByteEngine::Application::GetInstance().FatalCrash("Condition failed: " #condition); \
        }                                                                                         \
    } while (false)

#define BE_CHECK_MSG(condition, fmt, ...)                        \
    do                                                           \
    {                                                            \
        if (!(condition)) [[unlikely]]                           \
        {                                                        \
            ::ByteEngine::Application::GetInstance().FatalCrash( \
                "Condition failed: " #condition "\n"             \
                "Message: " fmt,                                 \
                ##__VA_ARGS__);                                  \
        }                                                        \
    } while (false)

#ifdef BE_DEBUG
    #define BE_DEBUG_LOG_INFO(fmt, ...) BE_LOG_INFO(fmt, ##__VA_ARGS__)
    #define BE_DEBUG_LOG_WARNING(fmt, ...) BE_LOG_WARNING(fmt, ##__VA_ARGS__)
    #define BE_DEBUG_LOG_ERROR(fmt, ...) BE_LOG_ERROR(fmt, ##__VA_ARGS__)
    #define BE_DEBUG_LOG_CRITICAL(fmt, ...) BE_LOG_CRITICAL(fmt, ##__VA_ARGS__)

    #define BE_DEBUG_CHECK(condition) BE_CHECK(condition)
    #define BE_DEBUG_CHECK_MSG(condition, fmt, ...) BE_CHECK_MSG(condition, fmt, ##__VA_ARGS__)

    #define BE_ENSURE(condition)                                                   \
        [&]() -> bool {                                                            \
            const bool conditionResult = condition;                                \
            static bool ensured = false;                                           \
                                                                                   \
            if (!ensured)                                                          \
            {                                                                      \
                ensured = true;                                                    \
                                                                                   \
                if (!conditionResult) [[unlikely]]                                 \
                {                                                                  \
                    const std::stacktrace backtrace = std::stacktrace::current(1); \
                    std::string backtraceString;                                   \
                    backtraceString.reserve(backtrace.size() * 20);                \
                    for (auto& el : backtrace)                                     \
                        backtraceString.append('\t' + std::to_string(el) + '\n');  \
                    BE_LOG_ERROR(                                                  \
                        "Condition failed: " #condition "\n"                       \
                        "Backtrace: \n{}\n",                                       \
                        backtraceString);                                          \
                }                                                                  \
            }                                                                      \
            ::ByteEngine::Debug::BreakpointIfDebuggerAttached();                   \
            return conditionResult;                                                \
        }()

    #define BE_ENSURE_MSG(condition, fmt, ...)                                     \
        [&]() -> bool {                                                            \
            const bool conditionResult = condition;                                \
            static bool ensured = false;                                           \
                                                                                   \
            if (!ensured)                                                          \
            {                                                                      \
                ensured = true;                                                    \
                                                                                   \
                if (!conditionResult) [[unlikely]]                                 \
                {                                                                  \
                    const std::stacktrace backtrace = std::stacktrace::current(1); \
                    std::string backtraceString;                                   \
                    backtraceString.reserve(backtrace.size() * 20);                \
                    for (auto& el : backtrace)                                     \
                        backtraceString.append('\t' + std::to_string(el) + '\n');  \
                    BE_LOG_ERROR(                                                  \
                        "Condition failed: " #condition "\n"                       \
                        "Message: " fmt "\n"                                       \
                        "Backtrace: \n{}\n",                                       \
                        ##__VA_ARGS__, backtraceString);                           \
                }                                                                  \
            }                                                                      \
            return conditionResult;                                                \
        }()

    #define BE_ENSURE_ALWAYS(condition)                                        \
        [&]() -> bool {                                                        \
            const bool conditionResult = condition;                            \
            if (!conditionResult) [[unlikely]]                                 \
            {                                                                  \
                const std::stacktrace backtrace = std::stacktrace::current(1); \
                std::string backtraceString;                                   \
                backtraceString.reserve(backtrace.size() * 20);                \
                for (auto& el : backtrace)                                     \
                    backtraceString.append('\t' + std::to_string(el) + '\n');  \
                BE_LOG_ERROR(                                                  \
                    "Condition failed: " #condition "\n"                       \
                    "Backtrace: \n{}\n",                                       \
                    backtraceString);                                          \
            }                                                                  \
            ::ByteEngine::Debug::BreakpointIfDebuggerAttached();               \
            return conditionResult;                                            \
        }()

    #define BE_ENSURE_ALWAYS_MSG(condition, fmt, ...)                          \
        [&]() -> bool {                                                        \
            const bool conditionResult = condition;                            \
            if (!conditionResult) [[unlikely]]                                 \
            {                                                                  \
                const std::stacktrace backtrace = std::stacktrace::current(1); \
                std::string backtraceString;                                   \
                backtraceString.reserve(backtrace.size() * 20);                \
                for (auto& el : backtrace)                                     \
                    backtraceString.append('\t' + std::to_string(el) + '\n');  \
                BE_LOG_ERROR(                                                  \
                    "Condition failed: " #condition "\n"                       \
                    "Message: " fmt "\n"                                       \
                    "Backtrace: \n{}\n",                                       \
                    ##__VA_ARGS__, backtraceString);                           \
            }                                                                  \
            ::ByteEngine::Debug::BreakpointIfDebuggerAttached();               \
            return conditionResult;                                            \
        }()
#else
    #define BE_DEBUG_LOG_INFO(fmt, ...) ((void)0)
    #define BE_DEBUG_LOG_WARNING(fmt, ...) ((void)0)
    #define BE_DEBUG_LOG_ERROR(fmt, ...) ((void)0)
    #define BE_DEBUG_LOG_CRITICAL(fmt, ...) ((void)0)

    #define BE_DEBUG_CHECK(condition) ((void)0)
    #define BE_DEBUG_CHECK_MSG(condition, fmt, ...) ((void)0)

    #define BE_ENSURE(condition) (!!(condition))
    #define BE_ENSURE_MSG(condition) (!!(condition))
    #define BE_ENSURE_ALWAYS(condition) (!!(condition))
    #define BE_ENSURE_ALWAYS_MSG(condition) (!!(condition))
#endif