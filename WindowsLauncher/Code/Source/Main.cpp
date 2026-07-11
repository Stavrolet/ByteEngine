#define BE_WINDOWS_MAIN

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "ByteEngine/Core/Base/Application.h"
#include "Win32Window.h"

using namespace ByteEngine;
using namespace ByteEngine::WindowsLauncher;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
    Win32Window window;
    window.Initialize("ByteEnigne", hInstance);
    MainWindow::SetInstance(&window);

    Application app;    
    return app.Run(window);
}