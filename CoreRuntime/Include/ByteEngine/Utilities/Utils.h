#pragma once

#include "ByteEngine/Primitives.h"

namespace ByteEngine::Utils
{
    template<typename T, size_t N>
    consteval size_t ArraySize(const T(&)[N])
    {
        return N;
    }
}