#pragma once

#include "ByteEngine/Core/Singleton.h"
#include "ByteEngine/Core/EventSystem/Delegate.h"
#include "ByteEngine/Core/EventSystem/MulticastDelegate.h"
#include "ByteEngine/Core/Input/KeyCode.h"
#include "ByteEngine/Math/Vector2.h"

#include <string>

namespace ByteEngine
{
    using namespace EventSystem;
    using namespace Math;

    enum class WindowMode : uint8
    {
        Minimized,
        Windowed,
        Mazimized,
        BorderlessFullscreen,
        ExclusiveFullscreen
    };

    class MainWindow : public Singleton<MainWindow>
    {
        friend class Application;
        friend class Input;

    protected:
        void* handle = nullptr;

        Vector2I size;
        Vector2I position;

        bool initialized = false;

        WindowMode mode = WindowMode::BorderlessFullscreen;
        WindowMode previousMode = mode;

        bool hasFocus = true;

        bool closeRequested = false;
        bool destroyed = false;

        std::string title;

        DelegateVoid<KeyCode, bool> keyStateChanged;
        DelegateVoid<Vector2I> mouseMoved;
        DelegateVoid<float, float> mouseWheelStateChanged;

        MulticastDelegate<Vector2I> resized;

        MulticastDelegate<bool> focusStateChanged;

        MulticastDelegate<WindowMode> modeChanged;
        MulticastDelegate<std::string_view> titleChanged;

    public:
        MulticastDelegate<Vector2I>& Resized() { return resized; }
        MulticastDelegate<bool>& FocusStateChanged() { return focusStateChanged; }
        MulticastDelegate<WindowMode>& ModeChanged() { return modeChanged; }
        MulticastDelegate<std::string_view>& TitleChanged() { return titleChanged; }

        ~MainWindow() override = default;

        virtual void Initialize(std::string windowName) = 0;

        virtual void SetWindowMode(WindowMode modeToSet) = 0;
        virtual void SetWindowTitle(std::string title) = 0;

        virtual void SetWindowSize(Vector2I size) { SetWindowSize(size.width, size.height); }
        virtual void SetWindowSize(int32 width, int32 height) = 0;

        virtual void SetWindowPosition(Vector2I position) { SetWindowPosition(position.x, position.y); }
        virtual void SetWindowPosition(int32 x, int32 y) = 0;

        virtual void SetFocus() = 0;

        void* GetNativeHandle() const { return handle; }

        WindowMode GetMode() const { return mode; }
        const std::string& GetTitle() const { return title; }

        Vector2I GetSize() const { return size; }
        Vector2I GetPosition() const { return position; }

        bool HasFocus() const { return hasFocus; }

    protected:
        virtual void Close() = 0;
        virtual void PollEvents() = 0;

    private:
        DelegateVoid<KeyCode, bool>& KeyStateChanged() { return keyStateChanged; }
        DelegateVoid<Vector2I>& MouseMoved() { return mouseMoved; }
        DelegateVoid<float, float>& MouseWheelStateChanged() { return mouseWheelStateChanged; }
    };
} // namespace ByteEngine