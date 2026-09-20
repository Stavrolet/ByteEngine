#include "ByteEngine/Math/Math.h"

namespace ByteEngine::Math::Mathf
{
    double Sin(RadianD rad) { return std::sin(rad.value); }
    double Cos(RadianD rad) { return std::cos(rad.value); }

    // Asin implementation adapted from DirectXMath (MIT License). See THIRDPARTY.md
    // Source: DirectX::XMScalarASin
    RadianF Asin(float value)
    {
        // Clamp input to [-1,1].
        bool nonnegative = (value >= 0.0f);
        float x = fabsf(value);
        float omx = 1.0f - x;
        if (omx < 0.0f)
        {
            omx = 0.0f;
        }
        float root = sqrtf(omx);

        // 7-degree minimax approximation
        float result = ((((((-0.0012624911f * x + 0.0066700901f) * x - 0.0170881256f) * x + 0.0308918810f) * x - 0.0501743046f) * x + 0.0889789874f) * x - 0.2145988016f) * x + 1.5707963050f;
        result *= root;  // acos(|x|)

        // acos(x) = pi - acos(-x) when x < 0, asin(x) = pi/2 - acos(x)
        return RadianF(nonnegative ? PI / 2.0f - result : result - PI / 2.0f);
    }

    RadianD Asin(double value) { return RadianD(std::asin(value)); }

    // Acos implementation adapted from DirectXMath (MIT License). See THIRDPARTY.md
    // Source: DirectX::XMScalarACos
    RadianF Acos(float value)
    {
        // Clamp input to [-1,1].
        bool nonnegative = (value >= 0.0f);
        float x = fabsf(value);
        float omx = 1.0f - x;
        if (omx < 0.0f)
        {
            omx = 0.0f;
        }
        float root = sqrtf(omx);

        // 7-degree minimax approximation
        float result = ((((((-0.0012624911f * x + 0.0066700901f) * x - 0.0170881256f) * x + 0.0308918810f) * x - 0.0501743046f) * x + 0.0889789874f) * x - 0.2145988016f) * x + 1.5707963050f;
        result *= root;

        // acos(x) = pi - acos(-x) when x < 0
        return RadianF(nonnegative ? result : PI - result);
    }

    RadianD Acos(double value) { return RadianD(std::acos(value)); }
}