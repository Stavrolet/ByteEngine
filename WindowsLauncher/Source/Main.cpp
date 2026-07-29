#define BE_WINDOWS_MAIN

#define WIN32_LEAN_AND_MEAN

// clang-format off
#include <Windows.h>

#include "ByteEngine/Core/Base/Application.h"
#include "Win32Window.h"
// clang-format on

using namespace ByteEngine;
using namespace ByteEngine::WindowsLauncher;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
    Win32Window window;
    window.Initialize("ByteEnigne", hInstance);
    MainWindow::SetInstance(&window);

    Application app;
    Application::Error error = app.Initialize();

    if (error != Application::Error::Success)
        return static_cast<int>(error);

    return app.Run();
}