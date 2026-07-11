#include <Windows.h>

#include "ByteEngine/Core/Base/MainWindow.h"
#include "ByteEngine/Core/Input/Input.h"
#include "ByteEngine/Debug.h"

namespace ByteEngine
{
    Input::Input()
        : Singleton()
    {
        actions = { { "test1", { KeyCode::A, KeyCode::MouseWheelDown, KeyCode::Aplha0, KeyCode::MouseMiddle, KeyCode::MouseWheelUp } } };
        
        keysState =
        {
            { KeyCode::LeftCtrl, false },
            { KeyCode::RightCtrl, false },
            { KeyCode::LeftShift, false },
            { KeyCode::RightShift, false },
            { KeyCode::LeftAlt, false },
            { KeyCode::RightAlt, false },
            { KeyCode::LeftGUI, false },
            { KeyCode::RightGUI, false },
            { KeyCode::Space, false },
            { KeyCode::Tab, false },
            { KeyCode::CapsLock, false },
            { KeyCode::Backspace, false },
            { KeyCode::Enter, false },
            { KeyCode::Insert, false },
            { KeyCode::Home, false },
            { KeyCode::PageUp, false },
            { KeyCode::Delete, false },
            { KeyCode::End, false },
            { KeyCode::PageDown, false },
            { KeyCode::RightArrow, false },
            { KeyCode::LeftArrow, false },
            { KeyCode::DownArrow, false },
            { KeyCode::UpArrow, false },
            { KeyCode::Escape, false },
            { KeyCode::F1, false },
            { KeyCode::F2, false },
            { KeyCode::F3, false },
            { KeyCode::F4, false },
            { KeyCode::F5, false },
            { KeyCode::F6, false },
            { KeyCode::F7, false },
            { KeyCode::F8, false },
            { KeyCode::F9, false },
            { KeyCode::F10, false },
            { KeyCode::F11, false },
            { KeyCode::F12, false },
            { KeyCode::F13, false },
            { KeyCode::F14, false },
            { KeyCode::F15, false },
            { KeyCode::F16, false },
            { KeyCode::F17, false },
            { KeyCode::F18, false },
            { KeyCode::F19, false },
            { KeyCode::F20, false },
            { KeyCode::F21, false },
            { KeyCode::F22, false },
            { KeyCode::F23, false },
            { KeyCode::F24, false },
            { KeyCode::PrintScreen, false },
            { KeyCode::ScrollLock, false },
            { KeyCode::Pause, false },
            { KeyCode::KeypadNumLock, false },
            { KeyCode::KeypadDivide, false },
            { KeyCode::KeypadMultiply, false },
            { KeyCode::KeypadMinus, false },
            { KeyCode::KeypadPlus, false },
            { KeyCode::KeypadEnter, false },
            { KeyCode::KeypadDelete, false },
            { KeyCode::Keypad0, false },
            { KeyCode::Keypad1, false },
            { KeyCode::Keypad2, false },
            { KeyCode::Keypad3, false },
            { KeyCode::Keypad4, false },
            { KeyCode::Keypad5, false },
            { KeyCode::Keypad6, false },
            { KeyCode::Keypad7, false },
            { KeyCode::Keypad8, false },
            { KeyCode::Keypad9, false },
            { KeyCode::BackQuoute, false },
            { KeyCode::Aplha1, false },
            { KeyCode::Aplha0, false },
            { KeyCode::Aplha2, false },
            { KeyCode::Aplha3, false },
            { KeyCode::Aplha4, false },
            { KeyCode::Aplha5, false },
            { KeyCode::Aplha6, false },
            { KeyCode::Aplha7, false },
            { KeyCode::Aplha8, false },
            { KeyCode::Aplha9, false },
            { KeyCode::Minus, false },
            { KeyCode::Equals, false },
            { KeyCode::LeftBracket, false },
            { KeyCode::RightBracket, false },
            { KeyCode::Slash, false },
            { KeyCode::Backslash, false },
            { KeyCode::Semicolon, false },
            { KeyCode::Quote, false },
            { KeyCode::Comma, false },
            { KeyCode::Period, false },
            { KeyCode::A, false },
            { KeyCode::B, false },
            { KeyCode::C, false },
            { KeyCode::D, false },
            { KeyCode::E, false },
            { KeyCode::F, false },
            { KeyCode::G, false },
            { KeyCode::H, false },
            { KeyCode::I, false },
            { KeyCode::J, false },
            { KeyCode::K, false },
            { KeyCode::L, false },
            { KeyCode::M, false },
            { KeyCode::N, false },
            { KeyCode::O, false },
            { KeyCode::P, false },
            { KeyCode::Q, false },
            { KeyCode::R, false },
            { KeyCode::S, false },
            { KeyCode::T, false },
            { KeyCode::U, false },
            { KeyCode::V, false },
            { KeyCode::W, false },
            { KeyCode::X, false },
            { KeyCode::Y, false },
            { KeyCode::Z, false },
            { KeyCode::MouseLeft, false },
            { KeyCode::MouseRight, false },
            { KeyCode::MouseMiddle, false },
            { KeyCode::MouseExtended1, false },
            { KeyCode::MouseExtended2, false },
            { KeyCode::MouseWheelUp, false },
            { KeyCode::MouseWheelDown, false },
            { KeyCode::MouseWheelLeft, false },
            { KeyCode::MouseWheelRight, false },
        };

        MainWindow::GetInstance().KeyStateChanged().SubscribeLambda([this](KeyCode code, bool isPressed)
        {
            keysState[code] = isPressed;
            isAnyKeyPressed = isPressed;
        });

        MainWindow::GetInstance().MouseMoved().SubscribeLambda([this](Vector2 delta) { mouseDelta += delta; });

        MainWindow::GetInstance().MouseWheelStateChanged().SubscribeLambda([this](float horizontalDelta, float verticalDelta)
        {
            horizontalWheelDelta += horizontalDelta;
            verticalWheelDelta += verticalDelta;
            isAnyKeyPressed = true;
        });
    }

    bool Input::IsActionPressed(std::string_view actionName) const
    {
        if (!actions.contains(actionName))
            return false;

        const auto& it = actions.find(actionName);

        for (KeyCode keyCode : it->second)
        {
            if (keysState.at(keyCode))
                return true;
        }

        return false;
    }

    bool Input::IsActionJustPressed(std::string_view actionName) const
    {
        if (!actions.contains(actionName))
            return false;

        const auto& it = actions.find(actionName);

        for (KeyCode keyCode : it->second)
        {
            if (!previousFrameKeysState.at(keyCode) && keysState.at(keyCode))
                return true;
        }

        return false;
    }

    bool Input::IsActionJustReleased(std::string_view actionName) const
    {
        if (!actions.contains(actionName))
            return false;

        const auto& it = actions.find(actionName);

        for (KeyCode keyCode : it->second)
        {
            if (previousFrameKeysState.at(keyCode) && !keysState.at(keyCode))
                return true;
        }

        return false;
    }

    bool Input::IsKeyPressed(KeyCode code) const { return keysState.at(code); }
    bool Input::IsKeyJustPressed(KeyCode code) const { return !previousFrameKeysState.at(code) && keysState.at(code); }
    bool Input::IsKeyJustReleased(KeyCode code) const { return previousFrameKeysState.at(code) && !keysState.at(code); }

    Vector2 Input::GetMousePosition() const
    {
        POINT pos = { };
        if (GetCursorPos(&pos) == false)
            BE_LOG_ERROR("Failed to get cursor position. Error code: {:X}", GetLastError());

        return Vector2(static_cast<float>(pos.x), static_cast<float>(pos.y));
    }

    void Input::Update()
    {
        previousFrameKeysState = keysState;
        keysState[KeyCode::Pause] = false;
        keysState[KeyCode::MouseWheelDown] = false;
        keysState[KeyCode::MouseWheelUp] = false;
        keysState[KeyCode::MouseWheelLeft] = false;
        keysState[KeyCode::MouseWheelRight] = false;
        isAnyKeyPressed = false;

        mouseDelta = Vector2::Zero();
        horizontalWheelDelta = 0.0f;
        verticalWheelDelta = 0.0f;
    }
}