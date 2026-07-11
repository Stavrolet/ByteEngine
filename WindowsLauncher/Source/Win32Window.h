#pragma once

#include "ByteEngine/WinApiExcludingDefs/LeanAndMean.h"
#undef NOSYSMETRICS
#undef NOVIRTUALKEYCODES

#include <Windows.h>

#include "ByteEngine/Core/Base/MainWindow.h"

using namespace ByteEngine::Math;

namespace ByteEngine::WindowsLauncher
{
    class Win32Window : public MainWindow
    {
        friend extern int WINAPI ::WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPTSTR, _In_ int);

    private:
        Vector2I accumulatedMouseDelta;
        float accumulatedHorizontalWheelDelta = 0.0f;
        float accumulatedVerticalWheelDelta = 0.0f;
        
    public:
        ~Win32Window() override;

        void Initialize(std::string windowName, HINSTANCE hInstance);
        void Close() override;

        void SetWindowMode(WindowMode modeToSet) override;
        void SetWindowTitle(std::string title) override;

        void SetWindowSize(int32 width, int32 height) override;
        void SetWindowPosition(int32 x, int32 y) override;

        void SetFocus() override;

    protected:
        void PollEvents() override;

    private:
        static LRESULT WINAPI StaticWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
        LRESULT WINAPI WndProc(UINT message, WPARAM wParam, LPARAM lParam);

        void HandleRawInputMessage(HRAWINPUT handle);
        void HandleWindowModeChangeMessage(WindowMode modeToSet);
    };
}