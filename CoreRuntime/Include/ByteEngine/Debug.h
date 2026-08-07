#pragma once

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
        ~Debug() { Shutdown(); }

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
#define BE_LOG_DEBUG(fmt, ...) QUILL_LOG_DEBUG(static_cast<::quill::Logger*>(::ByteEngine::Debug::GetInstance().GetLogger()), fmt, ##__VA_ARGS__)
#define BE_LOG_WARNING(fmt, ...) QUILL_LOG_WARNING(static_cast<::quill::Logger*>(::ByteEngine::Debug::GetInstance().GetLogger()), fmt, ##__VA_ARGS__)
#define BE_LOG_ERROR(fmt, ...) QUILL_LOG_ERROR(static_cast<::quill::Logger*>(::ByteEngine::Debug::GetInstance().GetLogger()), fmt, ##__VA_ARGS__)
#define BE_LOG_CRITICAL(fmt, ...) QUILL_LOG_CRITICAL(static_cast<::quill::Logger*>(::ByteEngine::Debug::GetInstance().GetLogger()), fmt, ##__VA_ARGS__)

#ifdef BE_DEBUG
    #define BE_ASSERT(condition, fmt, ...)       \
        if (!(condition))                        \
        {                                        \
            BE_LOG_CRITICAL(fmt, ##__VA_ARGS__); \
            ::ByteEngine::Debug::Breakpoint();   \
        }
#else
    #define BE_ASSERT(condition, fmt, ...) ((void)0)
#endif