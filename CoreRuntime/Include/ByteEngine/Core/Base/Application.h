#pragma once

#include "ByteEngine/Core/Base/Singleton.h"

#include "ByteEngine/Core/EventSystem/Delegate.h"
#include "ByteEngine/CoreTypes.h"
#include "ByteEngine/Debug.h"

#include <string_view>

namespace ByteEngine
{
    using namespace EventSystem;

    class MainWindow;

    class Application : public Singleton<Application>
    {
#ifdef BE_WINDOWS_MAIN
        friend int __stdcall ::WinMain(HINSTANCE, HINSTANCE, char*, int);
#endif

    public:
        enum class Error : int32
        {
            Success,
            GenericError,
            Unsupported,
        };

    private:
        int32 exitCode = 0;
        bool isRunning = true;

        Delegate<bool> quitRequest;

    public:
        void Quit(int32 exitCode);
        Delegate<bool>& QuitRequest() { return quitRequest; }

        template <typename... Args>
        [[noreturn]] void FatalCrash(std::string_view msg, Args... args) const
        {
            BE_LOG_CRITICAL(msg.data(), std::forward<Args>(args)...);

            if (Debug::IsDebuggerAttached())
                Debug::Breakpoint();
            else
                std::abort();
        }

    private:
        Error Initialize();
        int32 Run();
    };
} // namespace ByteEngine