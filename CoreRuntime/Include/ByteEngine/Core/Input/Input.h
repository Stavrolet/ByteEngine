#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "ByteEngine/Core/Base/Singleton.h"
#include "ByteEngine/Core/Input/KeyCode.h"
#include "ByteEngine/Math/Vector2.h"

namespace ByteEngine
{
    using namespace ByteEngine::Math;

    class MainWindow;

    class Input : public Singleton<Input>
    {
    private:
        friend class Application;

        struct StringHash
        {
            using is_transparent = void;

            size_t operator()(std::string_view sv) const noexcept
            {
                return std::hash<std::string_view>{}(sv);
            }
        };

        struct StringEqual
        {
            using is_transparent = void;

            bool operator()(std::string_view a, std::string_view b) const noexcept
            {
                return a == b;
            }
        };

        std::unordered_map<std::string, std::vector<KeyCode>, StringHash, StringEqual> actions;

        std::unordered_map<KeyCode, bool> keysState;
        std::unordered_map<KeyCode, bool> previousFrameKeysState;
        bool isAnyKeyPressed = false;

        Vector2 mouseDelta;
        float horizontalWheelDelta = 0.0f;
        float verticalWheelDelta = 0.0f;

        Input();

    public:
        bool IsActionPressed(std::string_view actionName) const;
        bool IsActionJustPressed(std::string_view actionName) const;
        bool IsActionJustReleased(std::string_view actionName) const;

        bool IsKeyPressed(KeyCode code) const;
        bool IsKeyJustPressed(KeyCode code) const;
        bool IsKeyJustReleased(KeyCode code) const;
        bool IsAnyKeyPressed() const { return isAnyKeyPressed; }

        Vector2 GetMouseDelta() const { return mouseDelta; }
        Vector2 GetMousePosition() const;

    private:
        void Update();
    };
}