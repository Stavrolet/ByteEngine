#pragma once

#include "ByteEngine/Math/Vector2.h"
#include "ByteEngine/Math/Vector3.h"
#include "ByteEngine/Math/Vector4.h"

namespace ByteEngine::Math
{
    template<Arithmetic T>
    Vector2T<T>::operator Vector3T<T>() const
    {
        return Vector3T<T>(x, y, 0);
    }

    template<Arithmetic T>
    template<Arithmetic U>
        requires (!std::is_same_v<T, U>&& std::is_convertible_v<T, U>)
    Vector2T<T>::operator Vector3T<U>() const
    {
        return Vector3T<U>(static_cast<U>(x), static_cast<U>(y), 0);
    }

    template<Arithmetic T>
    Vector2T<T>::operator Vector4T<T>() const
    {
        return Vector4T<T>(x, y, 0, 0);
    }

    template<Arithmetic T>
    template<Arithmetic U>
        requires (!std::is_same_v<T, U>&& std::is_convertible_v<T, U>)
    Vector2T<T>::operator Vector4T<U>() const
    {
        return Vector4T<U>(static_cast<U>(x), static_cast<U>(y), 0, 0);
    }

    template<Arithmetic T>
    Vector3T<T>::operator Vector2T<T>() const
    {
        return Vector2T(x, y);
    }

    template<Arithmetic T>
    template<Arithmetic U>
        requires (!std::is_same_v<T, U>&& std::is_convertible_v<T, U>)
    Vector3T<T>::operator Vector2T<U>() const
    {
        return Vector2T<U>(static_cast<U>(x), static_cast<U>(y));
    }

    template<Arithmetic T>
    Vector3T<T>::operator Vector4T<T>() const
    {
        return Vector4T<T>(x, y, z, 0);
    }

    template<Arithmetic T>
    template<Arithmetic U>
        requires (!std::is_same_v<T, U>&& std::is_convertible_v<T, U>)
    Vector3T<T>::operator Vector4T<U>() const
    {
        return Vector4T<U>(static_cast<U>(x), static_cast<U>(y), static_cast<U>(z), 0);
    }

    template<Arithmetic T>
    Vector4T<T>::operator Vector2T<T>() const
    {
        return Vector2T<T>(x, y);
    }

    template<Arithmetic T>
    template<Arithmetic U>
        requires (!std::is_same_v<T, U>&& std::is_convertible_v<T, U>)
    Vector4T<T>::operator Vector2T<U>() const
    {
        return Vector2T<U>(static_cast<U>(x), static_cast<U>(y));
    }

    template<Arithmetic T>
    Vector4T<T>::operator Vector3T<T>() const
    {
        return Vector3T<T>(x, y, z);
    }

    template<Arithmetic T>
    template<Arithmetic U>
        requires (!std::is_same_v<T, U>&& std::is_convertible_v<T, U>)
    Vector4T<T>::operator Vector3T<U>() const
    {
        return Vector3T<U>(static_cast<U>(x), static_cast<U>(y), static_cast<U>(z));
    }
}