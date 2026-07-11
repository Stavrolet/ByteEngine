#pragma once

#include "ByteEngine/Primitives.h"
#include "ByteEngine/Core/Base/Singleton.h"

namespace ByteEngine::Graphics
{
    DECLARE_HANDLE(Texture2D);

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