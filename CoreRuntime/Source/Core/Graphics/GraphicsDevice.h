#pragma once

#include "ByteEngine/Core/Base/Singleton.h"

namespace ByteEngine::Graphics
{
    class GraphicsDevice : public Singleton<GraphicsDevice>
    {
    public:
        enum class Error
        {
            Success,
            GenericError,
            Unsupported,
        };

        ~GraphicsDevice() override = default;

        virtual Error Initialize(bool enableDebug) = 0;
    };
}