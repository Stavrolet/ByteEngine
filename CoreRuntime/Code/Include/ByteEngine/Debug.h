#pragma once

#include "ByteEngine/Core/Base/Singleton.h"

#include <quill/Logger.h>
#include <quill/LogMacros.h>

namespace ByteEngine
{
    class Debug : public Singleton<Debug>
    {
        friend class Application;

    private:
        ::quill::Logger* logger = nullptr;

    public:
        ~Debug() { Shutdown(); }

        void* GetLogger() const 
        {
            return logger;
        }

    private:
        void Initialize();
        void Shutdown();
    };
}

#define BE_LOG_INFO(fmt, ...) QUILL_LOG_INFO(static_cast<::quill::Logger*>(::ByteEngine::Debug::GetInstance().GetLogger()), fmt, ##__VA_ARGS__)
#define BE_LOG_DEBUG(fmt, ...) QUILL_LOG_DEBUG(static_cast<::quill::Logger*>(::ByteEngine::Debug::GetInstance().GetLogger()), fmt, ##__VA_ARGS__)
#define BE_LOG_WARNING(fmt, ...) QUILL_LOG_WARNING(static_cast<::quill::Logger*>(::ByteEngine::Debug::GetInstance().GetLogger()), fmt, ##__VA_ARGS__)
#define BE_LOG_ERROR(fmt, ...) QUILL_LOG_ERROR(static_cast<::quill::Logger*>(::ByteEngine::Debug::GetInstance().GetLogger()), fmt, ##__VA_ARGS__)
#define BE_LOG_CRITICAL(fmt, ...) QUILL_LOG_CRITICAL(static_cast<::quill::Logger*>(::ByteEngine::Debug::GetInstance().GetLogger()), fmt, ##__VA_ARGS__)