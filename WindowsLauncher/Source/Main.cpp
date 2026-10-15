#define BE_WINDOWS_MAIN

#define WIN32_LEAN_AND_MEAN

// clang-format off
#include <Windows.h>
// clang-format on

#include "ByteEngine/Core/Application.h"
#include "ByteEngine/Core/Memory/Allocator.h"
#include "Win32Window.h"

using namespace ByteEngine;
using namespace ByteEngine::Memory;
using namespace ByteEngine::WindowsLauncher;

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
    Allocator* allocator = static_cast<Allocator*>(std::malloc(sizeof(Allocator)));
    new (allocator) Allocator();
    Allocator::SetInstance(allocator);

    Win32Window window;
    MainWindow::SetInstance(&window);

    Application app;
    Application::Error error = app.Initialize();

    if (error != Application::Error::Success)
        return static_cast<int>(error);

    return app.Run();
}