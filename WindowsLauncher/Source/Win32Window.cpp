#include "Win32Window.h"

#include "ByteEngine/Debug.h"
#include "ByteEngine/Math/Vector2.h"
#include "ByteEngine/Utilities/BitFlagsHelper.h"
#include "resources.h"

#include <Windows.h>
#include <format>
#include <hidusage.h>

#define WM_MODECHANGE (WM_APP + 1)

namespace ByteEngine::WindowsLauncher
{
    Win32Window::~Win32Window() { Close(); }

    void Win32Window::Initialize(std::string windowName, HINSTANCE hInstance)
    {
        WNDCLASSEX winClass = { };
        winClass.cbSize = sizeof(WNDCLASSEX);
        winClass.style = CS_HREDRAW | CS_VREDRAW;
        winClass.lpfnWndProc = StaticWndProc;
        winClass.hInstance = hInstance;
        winClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BYTEENGINED3D11));
        winClass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));
        winClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
        winClass.lpszClassName = windowName.data();

        RegisterClassEx(&winClass);

        handle = CreateWindowEx(
            WS_EX_APPWINDOW, windowName.data(), windowName.data(), WS_POPUP | WS_VISIBLE, 0, 0, 640,
            480, nullptr, nullptr, hInstance, this
        );

        ShowWindow(static_cast<HWND>(handle), SW_MAXIMIZE);

        HMONITOR hMonitor = MonitorFromWindow(static_cast<HWND>(handle), MONITOR_DEFAULTTONEAREST);

        MONITORINFO monitorInfo = { };
        monitorInfo.cbSize = sizeof(monitorInfo);
        GetMonitorInfo(hMonitor, &monitorInfo);

        int32 width = monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
        int32 height = monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;

        SetWindowPos(
            static_cast<HWND>(handle), nullptr, monitorInfo.rcMonitor.left,
            monitorInfo.rcMonitor.top, width, height,
            SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW
        );

        RAWINPUTDEVICE rids[] = {
            { .usUsagePage = HID_USAGE_PAGE_GENERIC,
              .usUsage = HID_USAGE_GENERIC_KEYBOARD,
              .dwFlags = 0,
              .hwndTarget = static_cast<HWND>(handle) },
            { .usUsagePage = HID_USAGE_PAGE_GENERIC,
              .usUsage = HID_USAGE_GENERIC_MOUSE,
              .dwFlags = 0,
              .hwndTarget = static_cast<HWND>(handle) }
        };

        RegisterRawInputDevices(rids, 2, sizeof(rids[0]));

        size.width = width;
        size.height = height;
        position.x = monitorInfo.rcMonitor.left;
        position.y = monitorInfo.rcMonitor.top;
        title = std::move(windowName);
        initialized = true;
    }

    void Win32Window::Close()
    {
        if (handle != nullptr)
        {
            DestroyWindow(static_cast<HWND>(handle));
            handle = nullptr;
        }
    }

    void Win32Window::SetWindowMode(WindowMode modeToSet)
    {
        if (mode == modeToSet)
            return;

        PostMessage(static_cast<HWND>(handle), WM_MODECHANGE, static_cast<WPARAM>(modeToSet), 0);
    }

    void Win32Window::SetWindowTitle(std::string title)
    {
        this->title = std::move(title);
        SetWindowText(static_cast<HWND>(handle), this->title.c_str());
        titleChanged.Invoke(this->title);
    }

    void Win32Window::SetWindowSize(int32 width, int32 height)
    {
        SetWindowPos(
            static_cast<HWND>(handle), nullptr, 0, 0, width, height,
            SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW | SWP_NOMOVE
        );

        size.width = width;
        size.height = height;
    }

    void Win32Window::SetWindowPosition(int32 x, int32 y)
    {
        SetWindowPos(
            static_cast<HWND>(handle), nullptr, x, y, 0, 0,
            SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW | SWP_NOSIZE
        );

        position.x = x;
        position.y = y;
    }

    void Win32Window::SetFocus() { SetForegroundWindow(static_cast<HWND>(handle)); }

    void Win32Window::PollEvents()
    {
        MSG msg = { };

        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                return;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        mouseMoved.Invoke(accumulatedMouseDelta);
        mouseWheelStateChanged.Invoke(accumulatedHorizontalWheelDelta, accumulatedVerticalWheelDelta);
    }

    LRESULT WINAPI Win32Window::StaticWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        Win32Window* window = nullptr;

        if (msg == WM_CREATE)
        {
            CREATESTRUCT* createStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
            window = reinterpret_cast<Win32Window*>(createStruct->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
            return 0;
        }
        else
        {
            window = reinterpret_cast<Win32Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        }

        if (window != nullptr)
            return window->WndProc(msg, wParam, lParam);

#ifdef _DEBUG
        if (msg != WM_NCCREATE && msg != WM_NCCALCSIZE)
            OutputDebugStringA(std::format("[WARNING] window is nullptr in StaticWndProc. Message = {}.", msg).c_str());
#endif

        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    LRESULT WINAPI Win32Window::WndProc(UINT msg, WPARAM wParam, LPARAM lParam)
    {
        switch (msg)
        {
        case WM_DESTROY:
            destroyed = true;
            PostQuitMessage(0);
            return 0;
        case WM_MOVE:
            position.x = LOWORD(lParam);
            position.y = HIWORD(lParam);
            return 0;
        case WM_SIZE:
            size.width = LOWORD(lParam);
            size.height = HIWORD(lParam);

            if (initialized)
            {
                if (wParam == SIZE_MINIMIZED)
                {
                    previousMode = mode;
                    mode = WindowMode::Minimized;
                    return 0;
                }
                else if (wParam == SIZE_MAXIMIZED)
                {
                    mode = WindowMode::Mazimized;
                    BE_LOG_INFO("WM_SIZE received. wParam = SIZE_MAXIMIZED");
                }
                else if (wParam == SIZE_RESTORED)
                {
                    mode = WindowMode::Windowed;
                }

                resized.Invoke(size);
            }

            return 0;
        case WM_ACTIVATE:
            if (LOWORD(wParam) == WA_INACTIVE)
            {
                hasFocus = false;
                if (mode == WindowMode::ExclusiveFullscreen)
                    SetWindowMode(WindowMode::Minimized);
            }
            else
            {
                hasFocus = true;
                if (mode == WindowMode::Minimized)
                    SetWindowMode(previousMode);
            }

            focusStateChanged.Invoke(hasFocus);
            return 0;
        case WM_CLOSE:
            closeRequested = true;
            return 0;
        case WM_INPUT:
            HandleRawInputMessage(reinterpret_cast<HRAWINPUT>(lParam));
            return 0;
        case WM_KEYDOWN:
            if (wParam == VK_ESCAPE)
                CloseWindow(static_cast<HWND>(handle));
            else if (wParam == '1')
                SetWindowMode(WindowMode::Windowed);
            else if (wParam == '2')
                SetWindowMode(WindowMode::Mazimized);
            else if (wParam == '3')
                SetWindowMode(WindowMode::BorderlessFullscreen);
            else if (wParam == '4')
                SetWindowMode(WindowMode::ExclusiveFullscreen);
            return 0;
        case WM_MODECHANGE:
            HandleWindowModeChangeMessage(static_cast<WindowMode>(wParam));
            return 0;
        }

        return DefWindowProc(static_cast<HWND>(handle), msg, wParam, lParam);
    }

    void Win32Window::HandleRawInputMessage(HRAWINPUT handle)
    {
        uint32 size = 0;
        GetRawInputData(handle, RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER));

        std::vector<uint8> buffer(size);
        GetRawInputData(handle, RID_INPUT, buffer.data(), &size, sizeof(RAWINPUTHEADER));

        RAWINPUT* raw = (RAWINPUT*)buffer.data();

        if (raw->header.dwType == RIM_TYPEKEYBOARD)
        {
            const RAWKEYBOARD& keyboard = raw->data.keyboard;
            uint16 scanCode = 0;
            bool isKeyPressed = !BitFlags::HasOneFlag(keyboard.Flags, (uint16)RI_KEY_BREAK);

            if (keyboard.MakeCode == KEYBOARD_OVERRUN_MAKE_CODE)
                return;

            if (keyboard.VKey == VK_PAUSE)
            {
                keyStateChanged.Invoke(KeyCode::Pause, true);
                return;
            }

            if (keyboard.MakeCode != 0)
            {
                uint16 scanCode = MAKEWORD(
                    keyboard.MakeCode & 0x7f,
                    (BitFlags::HasOneFlag(keyboard.Flags, (USHORT)RI_KEY_E0)
                         ? 0xe0
                         : (BitFlags::HasOneFlag(keyboard.Flags, (USHORT)RI_KEY_E1) ? 0xe1 : 0x00))
                );

                keyStateChanged.Invoke(static_cast<KeyCode>(scanCode), isKeyPressed);
            }

            char keyNameBuffer[MAX_PATH] = { };
            GetKeyNameTextA(
                (LONG)MAKELPARAM(0, (HIBYTE(scanCode) ? KF_EXTENDED : 0x00) | LOBYTE(scanCode)),
                keyNameBuffer, MAX_PATH
            );

            BE_LOG_INFO("Key pressed. Name: {}, is pressed = {}", keyNameBuffer, isKeyPressed);
        }
        else if (raw->header.dwType == RIM_TYPEMOUSE)
        {
            const RAWMOUSE& mouse = raw->data.mouse;

            if (BitFlags::HasOneFlag(mouse.usFlags, (uint16)MOUSE_MOVE_RELATIVE))
                accumulatedMouseDelta += Vector2I(mouse.lLastX, mouse.lLastY);

            if (BitFlags::HasOneFlag(mouse.usButtonFlags, (uint16)RI_MOUSE_WHEEL))
            {
                float delta = static_cast<int16>(mouse.usButtonData) / (float)WHEEL_DELTA;
                accumulatedVerticalWheelDelta += delta;

                if (delta > 0.0f)
                    keyStateChanged.Invoke(KeyCode::MouseWheelUp, true);
                else
                    keyStateChanged.Invoke(KeyCode::MouseWheelDown, true);
            }
            else if (BitFlags::HasOneFlag(mouse.usButtonFlags, (uint16)RI_MOUSE_HWHEEL))
            {
                float delta = static_cast<int16>(mouse.usButtonData) / (float)WHEEL_DELTA;
                accumulatedHorizontalWheelDelta += delta;

                if (delta > 0)
                    keyStateChanged.Invoke(KeyCode::MouseWheelRight, true);
                else
                    keyStateChanged.Invoke(KeyCode::MouseWheelLeft, true);
            }

            if (BitFlags::HasOneFlag(mouse.usButtonFlags, (uint16)RI_MOUSE_LEFT_BUTTON_DOWN))
                keyStateChanged.Invoke(KeyCode::MouseLeft, true);

            if (BitFlags::HasOneFlag(mouse.usButtonFlags, (uint16)RI_MOUSE_LEFT_BUTTON_UP))
                keyStateChanged.Invoke(KeyCode::MouseLeft, false);

            if (BitFlags::HasOneFlag(mouse.usButtonFlags, (uint16)RI_MOUSE_RIGHT_BUTTON_DOWN))
                keyStateChanged.Invoke(KeyCode::MouseRight, true);

            if (BitFlags::HasOneFlag(mouse.usButtonFlags, (uint16)RI_MOUSE_RIGHT_BUTTON_UP))
                keyStateChanged.Invoke(KeyCode::MouseRight, false);

            if (BitFlags::HasOneFlag(mouse.usButtonFlags, (uint16)RI_MOUSE_MIDDLE_BUTTON_DOWN))
                keyStateChanged.Invoke(KeyCode::MouseMiddle, true);

            if (BitFlags::HasOneFlag(mouse.usButtonFlags, (uint16)RI_MOUSE_MIDDLE_BUTTON_UP))
                keyStateChanged.Invoke(KeyCode::MouseMiddle, false);

            if (BitFlags::HasOneFlag(mouse.usButtonFlags, (uint16)RI_MOUSE_BUTTON_4_DOWN))
                keyStateChanged.Invoke(KeyCode::MouseExtended1, true);

            if (BitFlags::HasOneFlag(mouse.usButtonFlags, (uint16)RI_MOUSE_BUTTON_4_UP))
                keyStateChanged.Invoke(KeyCode::MouseExtended1, false);

            if (BitFlags::HasOneFlag(mouse.usButtonFlags, (uint16)RI_MOUSE_BUTTON_5_DOWN))
                keyStateChanged.Invoke(KeyCode::MouseExtended2, true);

            if (BitFlags::HasOneFlag(mouse.usButtonFlags, (uint16)RI_MOUSE_BUTTON_5_UP))
                keyStateChanged.Invoke(KeyCode::MouseExtended2, false);
        }
    }

    void Win32Window::HandleWindowModeChangeMessage(WindowMode modeToSet)
    {
        switch (modeToSet)
        {
        case WindowMode::Windowed:
        case WindowMode::Mazimized:
            if (mode == WindowMode::BorderlessFullscreen || mode == WindowMode::ExclusiveFullscreen)
            {
                DWORD style = static_cast<DWORD>(GetWindowLongPtr(static_cast<HWND>(handle), GWL_STYLE));

                BitFlags::ClearFlags(style, WS_POPUP | WS_VISIBLE);
                BitFlags::SetFlags(style, WS_OVERLAPPEDWINDOW);

                if (SetWindowLongPtr(static_cast<HWND>(handle), GWL_STYLE, style) == 0)
                {
                    BE_LOG_DEBUG("SetWindowLongPtr error: {}", GetLastError());
                    return;
                }
            }

            ShowWindow(
                static_cast<HWND>(handle),
                modeToSet == WindowMode::Windowed ? SW_RESTORE : SW_MAXIMIZE
            );
            break;
        case WindowMode::BorderlessFullscreen:
        case WindowMode::ExclusiveFullscreen:
            if (mode == WindowMode::Windowed || mode == WindowMode::Mazimized)
            {
                MONITORINFO monInfo = { };
                monInfo.cbSize = sizeof(MONITORINFO);

                int32 width = 0;
                int32 height = 0;
                int32 posX = 0;
                int32 posY = 0;

                if (GetMonitorInfo(
                        MonitorFromWindow(static_cast<HWND>(handle), MONITOR_DEFAULTTONEAREST),
                        &monInfo
                    ) != 0)
                {
                    width = monInfo.rcMonitor.right - monInfo.rcMonitor.left;
                    height = monInfo.rcMonitor.bottom - monInfo.rcMonitor.top;
                    posX = monInfo.rcMonitor.left;
                    posY = monInfo.rcMonitor.top;
                }
                else
                {
                    BE_LOG_DEBUG("GetMonitorInfo error: {}", GetLastError());
                    width = GetSystemMetrics(SM_CXSCREEN);
                    height = GetSystemMetrics(SM_CYSCREEN);
                }

                DWORD style = static_cast<DWORD>(GetWindowLongPtr(static_cast<HWND>(handle), GWL_STYLE));

                BitFlags::ClearFlags(style, WS_OVERLAPPEDWINDOW);
                BitFlags::SetFlags(style, WS_POPUP);

                if (SetWindowLongPtr(static_cast<HWND>(handle), GWL_STYLE, style) == 0)
                {
                    BE_LOG_DEBUG("SetWindowLongPtr error: {}", GetLastError());
                    return;
                }

                if (SetWindowPos(
                        static_cast<HWND>(handle), nullptr, posX, posY, width, height,
                        SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW
                    ) == 0)
                {
                    BE_LOG_DEBUG("SetWindowPos error: {}", GetLastError());
                    return;
                }
            }
            break;
        case WindowMode::Minimized:
            ShowWindow(static_cast<HWND>(handle), SW_MINIMIZE);
            break;
        }

        mode = modeToSet;
        modeChanged.Invoke(modeToSet);
    }
} // namespace ByteEngine::WindowsLauncher