#pragma once

#include <charconv>
#include <concepts>
#include <format>
#include <optional>
#include <string>

#include "ByteEngine/Math/Math.h"
#include "ByteEngine/Math/Vector4.h"

namespace ByteEngine::Math
{
    template<std::floating_point T>
    struct ColorT
    {
        union
        {
            struct
            {
                T r;
                T g;
                T b;
                T a;
            };

            T data[4];
        };

        explicit constexpr ColorT(T rgb, T a = 1)
            : r(rgb), g(rgb), b(rgb), a(a)
        { }

        constexpr ColorT(T r, T g, T b, T a = 1)
            : r(r), g(g), b(b), a(a)
        { }

        constexpr ColorT(uint8 r, uint8 g, uint8 b, uint8 a = 255)
            : r(r / T(255)), g(g / T(255)), b(b / T(255)), a(a / T(255))
        { }

        explicit constexpr ColorT(const T arr[4])
            : r(arr[0]), g(arr[1]), b(arr[2]), a(arr[3])
        { }

        explicit constexpr ColorT(const uint8 arr[4])
            : r(arr[0] / T(255)), g(arr[1] / T(255)), b(arr[2] / T(255)), a(arr[3] / T(255))
        { }

        constexpr void ToLinear()
        {
            r = Math::GammaToLinearSpace(r);
            g = Math::GammaToLinearSpace(g);
            b = Math::GammaToLinearSpace(b);
        }

        [[nodiscard]] constexpr ColorT AsLinear() const
        {
            return ColorT {
                Math::GammaToLinearSpace(r),
                Math::GammaToLinearSpace(g),
                Math::GammaToLinearSpace(b),
                a
            };
        }

        constexpr void ToGamma()
        {
            r = Math::LinearToGammaSpace(r);
            g = Math::LinearToGammaSpace(g);
            b = Math::LinearToGammaSpace(b);
        }

        [[nodiscard]] constexpr ColorT AsGamma() const
        {
            return ColorT {
                Math::LinearToGammaSpace(r),
                Math::LinearToGammaSpace(g),
                Math::LinearToGammaSpace(b),
                a
            };
        }

        constexpr void Clamp(T min = 0, T max = 1)
        {
            r = Math::Clamp(r, min, max);
            g = Math::Clamp(g, min, max);
            b = Math::Clamp(b, min, max);
            a = Math::Clamp(a, min, max);
        }

        [[nodiscard]] constexpr ColorT Clamped(T min = 0, T max = 1) const
        {
            return ColorT {
                Math::Clamp(r, min, max),
                Math::Clamp(g, min, max),
                Math::Clamp(b, min, max),
                Math::Clamp(a, min, max)
            };
        }

        [[nodiscard]] constexpr T MaxColorComponent() const { return Math::Max(r, g, b); }

        [[nodiscard]] std::optional<std::string> ToString() const
        {
            return ToString("F5A");
        }

        [[nodiscard]] std::optional<std::string> ToString(std::string_view format) const
        {
            if (format[0] == 'H')
            {
                uint8 r8 = static_cast<uint8>(Math::Round(Math::Clamp(r * 255, 0, 255)));
                uint8 g8 = static_cast<uint8>(Math::Round(Math::Clamp(g * 255, 0, 255)));
                uint8 b8 = static_cast<uint8>(Math::Round(Math::Clamp(b * 255, 0, 255)));
                uint8 a8 = static_cast<uint8>(Math::Round(Math::Clamp(a * 255, 0, 255)));

                if (format.size() > 1 && format[1] == 'A')
                    return std::format("#{0:02X}{1:02X}{2:02X}{3:02X}", r8, g8, b8, a8);
                else
                    return std::format("#{0:02X}{1:02X}{2:02X}", r8, g8, b8);
            }
            else if (format[0] == 'F')
            {
                if (format.size() > 1 && format[format.size() - 1] == 'A')
                {
                    int32 precision = 0;
                    auto result = std::from_chars(format.data() + 1, format.data() + format.size() - 1, precision);

                    if (result.ec != std::errc())
                        return std::nullopt;

                    return std::format(
                        "Color({:.{}f}, {:.{}f}, {:.{}f}, {:.{}f})",
                        r, precision,
                        g, precision,
                        b, precision,
                        a, precision
                    );
                }
                else
                {
                    int32 precision = 0;
                    auto result = std::from_chars(format.data() + 1, format.data() + format.size(), precision);

                    if (result.ec != std::errc())
                        return std::nullopt;

                    return std::format(
                        "Color({:.{}f}, {:.{}f}, {:.{}f})",
                        r, precision,
                        g, precision,
                        b, precision
                    );
                }
            }
            else
            {
                return std::nullopt;
            }
        }

        [[nodiscard]] static ColorT HsvToRgb(T h, T s, T v, T a = 1)
        {
            if (s == 0)
            {
                return ColorT { v, a };
            }
            else
            {
                h = Math::IsEqualApproximetly(h, T(360)) ? 0 : h / T(60);

                int32 i = static_cast<int32>(Math::Floor(h));

                T f = h - i;
                T p = v * (T(1) - s);
                T q = v * (T(1) - (s * f));
                T t = v * (T(1) - (s * (T(1) - f)));

                switch (i)
                {
                case 0:
                    return ColorT { v, t, p, a };
                    break;
                case 1:
                    return ColorT { q, v, p, a };
                    break;
                case 2:
                    return ColorT { p, v, t, a };
                    break;
                case 3:
                    return ColorT { p, q, v, a };
                    break;
                case 4:
                    return ColorT { t, p, v, a };
                    break;
                default:
                    return ColorT { v, p, q, a };
                    break;
                }
            }
        }

        static void RgbToHsv(ColorT color, T& h, T& s, T& v)
        {
            T max = Math::Max(color.r, color.g, color.b);
            T min = Math::Min(color.r, color.g, color.b);

            v = max;
            T delta = max - min;

            if (delta < Math::Epsilon)
            {
                h = T(0);
                s = T(0);
            }
            else
            {
                s = delta / max;

                if (color.r >= max)
                    h = (color.g - color.b) / delta;
                else if (color.g >= max)
                    h = T(2) + (color.b - color.r) / delta;
                else
                    h = T(4) + (color.r - color.g) / delta;

                h *= T(60);

                if (h < T(0))
                    h += T(360);
            }
        }

        [[nodiscard]] static std::optional<ColorT> HtmlToRgb(std::string_view html)
        {
            if (html.empty())
                return std::nullopt;

            if (html.size() != 6 && html.size() != 7 && html.size() != 8 && html.size() != 9)
                return std::nullopt;

            if (html.find_first_of(" \t\n\v\f\r") != std::string_view::npos)
                return std::nullopt;

            if (html[0] == '#')
            {
                return HtmlToRgb(html.substr(1));
            }
            else
            {
                uint32 value;
                auto [ptr, ec] = std::from_chars(html.data(), html.data() + html.size(), value, 16);

                if (ec == std::errc())
                {
                    if (html.size() == 6)
                    {
                        return ColorT {
                            ((value >> 16) & 0xFF) / T(255),
                            ((value >> 8) & 0xFF) / T(255),
                            (value & 0xFF) / T(255)
                        };
                    }
                    else if (html.size() == 8)
                    {
                        return ColorT {
                            ((value >> 24) & 0xFF) / T(255),
                            ((value >> 16) & 0xFF) / T(255),
                            ((value >> 8) & 0xFF) / T(255),
                            (value & 0xFF) / T(255)
                        };
                    }
                }
                else
                {
                    return std::nullopt;
                }
            }

            return std::nullopt;
        }

        [[nodiscard]] static ColorT Lerp(ColorT from, ColorT to, T t)
        {
            return from + (to - from) * t;
        }

        [[nodiscard]] static ColorT LerpClamped(ColorT from, ColorT to, T t)
        {
            return from + (to - from) * Math::Clamp(t);
        }

        [[nodiscard]] static constexpr bool IsEqualApproximetly(ColorT a, ColorT b, T tolerance = Math::Epsilon)
        {
            return Math::IsEqualApproximetly(a.r, b.r, tolerance) && Math::IsEqualApproximetly(a.g, b.g, tolerance) && Math::IsEqualApproximetly(a.b, b.b, tolerance) && Math::IsEqualApproximetly(a.a, b.a, tolerance);
        }

        [[nodiscard]] static constexpr ColorT Min(ColorT a, ColorT b)
        {
            return ColorT {
                Math::Min(a.r, b.r),
                Math::Min(a.g, b.g),
                Math::Min(a.b, b.b),
                Math::Min(a.a, b.a)
            };
        }

        [[nodiscard]] static constexpr ColorT Min(ColorT a, ColorT b, ColorT c)
        {
            return Min(Min(a, b), c);
        }

        [[nodiscard]] static constexpr ColorT Max(ColorT a, ColorT b)
        {
            return ColorT {
                Math::Max(a.r, b.r),
                Math::Max(a.g, b.g),
                Math::Max(a.b, b.b),
                Math::Max(a.a, b.a)
            };
        }

        [[nodiscard]] static constexpr ColorT Max(ColorT a, ColorT b, ColorT c)
        {
            return Max(Max(a, b), c);
        }

        [[nodiscard]] constexpr ColorT operator+() const { return ColorT { +r, +g, +b, +a }; }
        [[nodiscard]] constexpr ColorT operator-() const { return ColorT { -r, -g, -b, -a }; }

        [[nodiscard]] constexpr ColorT operator+(ColorT other) const { return ColorT { r + other.r, g + other.g, b + other.b, a + other.a }; }
        [[nodiscard]] constexpr ColorT operator-(ColorT other) const { return ColorT { r - other.r, g - other.g, b - other.b, a - other.a }; }

        constexpr ColorT& operator+=(ColorT other)
        {
            *this = *this + other;
            return *this;
        }

        constexpr ColorT& operator-=(ColorT other)
        {
            *this = *this - other;
            return *this;
        }

        [[nodiscard]] constexpr ColorT operator*(ColorT other) const { return ColorT { r * other.r, g * other.g, b * other.b, a * other.a }; }
        [[nodiscard]] constexpr ColorT operator/(ColorT other) const { return ColorT { r / other.r, g / other.g, b / other.b, a / other.a }; }

        constexpr ColorT& operator*=(ColorT other)
        {
            *this = *this * other;
            return *this;
        }

        constexpr ColorT& operator/=(ColorT other)
        {
            *this = *this / other;
            return *this;
        }

        [[nodiscard]] constexpr ColorT operator*(T scalar) const { return ColorT { r * scalar, g * scalar, b * scalar, a * scalar }; }
        [[nodiscard]] friend constexpr ColorT operator*(T scalar, ColorT color) { return color * scalar; }
        [[nodiscard]] constexpr ColorT operator/(T scalar) const { return ColorT { r / scalar, g / scalar, b / scalar, a / scalar }; }

        constexpr ColorT& operator*=(T scalar)
        {
            *this = *this * scalar;
            return *this;
        }

        constexpr ColorT& operator/=(T scalar)
        {
            *this = *this / scalar;
            return *this;
        }

        [[nodiscard]] constexpr bool operator==(ColorT other) const { return r == other.r && g == other.g && b == other.b && a == other.a; }
        [[nodiscard]] constexpr bool operator!=(ColorT other) const { return !(*this == other); }

        [[nodiscard]] constexpr T& operator[](int32 index)
        {
            assert(index >= 0 && index < 4);
            return data[index];
        }

        [[nodiscard]] constexpr T operator[](int32 index) const
        {
            assert(index >= 0 && index < 4);
            return data[index];
        }

        template<std::floating_point U>
            requires (!std::is_same_v<T, U>)
        [[nodiscard]] constexpr operator ColorT<U>() const { return ColorT<U>(static_cast<U>(r), static_cast<U>(g), static_cast<U>(b), static_cast<U>(a)); }

        template<std::floating_point U>
        [[nodiscard]] constexpr operator Vector4T<U>() const { return Vector4T<U>(static_cast<U>(r), static_cast<U>(g), static_cast<U>(b), static_cast<U>(a)); }

        static const ColorT Black;
        static const ColorT White;
        static const ColorT Red;
        static const ColorT Green;
        static const ColorT Blue;
    };

    template<std::floating_point T>
    inline constexpr ColorT<T> ColorT<T>::Black { 0 };

    template<std::floating_point T>
    inline constexpr ColorT<T> ColorT<T>::White { 1 };

    template<std::floating_point T>
    inline constexpr ColorT<T> ColorT<T>::Red { 1, 0, 0 };

    template<std::floating_point T>
    inline constexpr ColorT<T> ColorT<T>::Green { 0, 1, 0 };

    template<std::floating_point T>
    inline constexpr ColorT<T> ColorT<T>::Blue { 0, 0, 1 };

    using ColorF = ColorT<float>;
    using ColorD = ColorT<double>;
    using Color = ColorF;
}