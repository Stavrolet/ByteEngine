#pragma once

#define NOMINMAX

#include "ByteEngine/Core/Base/Application.h"
#include "ByteEngine/Core/Base/Singleton.h"

#include <quill/LogMacros.h>
#include <quill/Logger.h>

namespace ByteEngine
{
    class Debug : public Singleton<Debug>
    {
        friend class Application;

    private:
        ::quill::Logger* logger = nullptr;

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

#ifdef BE_DEBUG
    #define BE_DEBUG_LOG_INFO(fmt, ...) BE_LOG_INFO(fmt, ##__VA_ARGS__)
    #define BE_DEBUG_LOG_WARNING(fmt, ...) BE_LOG_WARNING(fmt, ##__VA_ARGS__)
    #define BE_DEBUG_LOG_ERROR(fmt, ...) BE_LOG_ERROR(fmt, ##__VA_ARGS__)
    #define BE_DEBUG_LOG_CRITICAL(fmt, ...) BE_LOG_CRITICAL(fmt, ##__VA_ARGS__)

    #define BE_ASSERT(condition)                                                                                          \
        do                                                                                                                \
        {                                                                                                                 \
            if (!(condition))                                                                                             \
            {                                                                                                             \
                ::ByteEngine::Application::GetInstance().FatalCrash("Assertion failed: " #condition ". "); \
            }                                                                                                             \
        } while (false)

    #define BE_ASSERT_MSG(condition, fmt, ...)                                                                                               \
        do                                                                                                                                   \
        {                                                                                                                                    \
            if (!(condition))                                                                                                                \
            {                                                                                                                                \
                ::ByteEngine::Application::GetInstance().FatalCrash(std::string("Assertion failed: " #condition ". ") + fmt, ##__VA_ARGS__); \
            }                                                                                                                                \
        } while (false)
#else
    #define BE_DEBUG_LOG_INFO(fmt, ...) ((void)0)
    #define BE_DEBUG_LOG_WARNING(fmt, ...) ((void)0)
    #define BE_DEBUG_LOG_ERROR(fmt, ...) ((void)0)
    #define BE_DEBUG_LOG_CRITICAL(fmt, ...) ((void)0)

    #define BE_ASSERT(condition) ((void)0)
    #define BE_ASSERT_MSG(condition, fmt, ...) ((void)0)
#endif