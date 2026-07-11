#pragma once

#include "ByteEngine/Core/Base/Singleton.h"
#include "ByteEngine/Core/EventSystem/Delegate.h"
#include "ByteEngine/Primitives.h"

#ifdef BE_WINDOWS_MAIN
struct HINSTANCE__;
using HINSTANCE = HINSTANCE__*;
#endif

namespace ByteEngine
{
    using namespace EventSystem;

    class MainWindow;

    class Application : public Singleton<Application>
    {
#ifdef BE_WINDOWS_MAIN
        friend int __stdcall ::WinMain(HINSTANCE, HINSTANCE, char*, int);
#endif

    private:
        int32 exitCode = 0;
        bool isRunning = true;

        Delegate<bool> quitRequest;

    public:
        void Quit(int32 exitCode);
        Delegate<bool>& QuitRequest() { return quitRequest; }

    private:
        int32 Run(MainWindow& mainWindow);
    };
}