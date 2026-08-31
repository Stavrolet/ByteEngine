#pragma once

#include "ByteEngine/Core/Base/Singleton.h"
#include "ByteEngine/Core/EventSystem/Delegate.h"
#include "ByteEngine/CoreTypes.h"

#include <format>
#include <string>
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
        [[noreturn]] void FatalCrash(std::format_string<Args...> msg, Args... args) const
        {
            const std::string format = std::format(msg, std::forward<Args>(args)...);
            FatalCrashImpl(format);
        }

    private:
        Error Initialize();
        int32 Run();

        void FatalCrashImpl(std::string_view msg) const;
    };
} // namespace ByteEngine