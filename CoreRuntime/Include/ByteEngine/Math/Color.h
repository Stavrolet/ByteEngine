#pragma once

#include "ByteEngine/Debug.h"
#include "ByteEngine/Math/Math.h"

#include <charconv>
#include <concepts>
#include <optional>
#include <string>

namespace ByteEngine::Math
{
    template <std::floating_point T>
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

        constexpr ColorT() = default;

        explicit constexpr ColorT(T rgb, T a = 1) :
            r(rgb), g(rgb), b(rgb), a(a)
        { }

        constexpr ColorT(T r, T g, T b, T a = 1) :
            r(r), g(g), b(b), a(a)
        { }

        constexpr ColorT(uint8 r, uint8 g, uint8 b, uint8 a = 255) :
            r(r / T(255)), g(g / T(255)), b(b / T(255)), a(a / T(255))
        { }

        explicit constexpr ColorT(const T arr[4]) :
            r(arr[0]), g(arr[1]), b(arr[2]), a(arr[3])
        { }

        explicit constexpr ColorT(const uint8 arr[4]) :
            r(arr[0] / T(255)), g(arr[1] / T(255)), b(arr[2] / T(255)), a(arr[3] / T(255))
        { }

        template <std::floating_point U>
            requires(!std::is_same_v<T, U>)
        explicit constexpr ColorT(ColorT<U> other) :
            r(static_cast<T>(other.r)), g(static_cast<T>(other.g)), b(static_cast<T>(other.b)), a(static_cast<T>(other.a))
        { }

        constexpr void ToLinear()
        {
            r = Mathf::GammaToLinearSpace(r);
            g = Mathf::GammaToLinearSpace(g);
            b = Mathf::GammaToLinearSpace(b);
        }

        [[nodiscard]] constexpr ColorT AsLinear() const
        {
            return ColorT {
                Mathf::GammaToLinearSpace(r),
                Mathf::GammaToLinearSpace(g),
                Mathf::GammaToLinearSpace(b),
                a
            };
        }

        constexpr void ToGamma()
        {
            r = Mathf::LinearToGammaSpace(r);
            g = Mathf::LinearToGammaSpace(g);
            b = Mathf::LinearToGammaSpace(b);
        }

        [[nodiscard]] constexpr ColorT AsGamma() const
        {
            return ColorT {
                Mathf::LinearToGammaSpace(r),
                Mathf::LinearToGammaSpace(g),
                Mathf::LinearToGammaSpace(b),
                a
            };
        }

        constexpr void Clamp(T min = 0, T max = 1)
        {
            r = Mathf::Clamp(r, min, max);
            g = Mathf::Clamp(g, min, max);
            b = Mathf::Clamp(b, min, max);
            a = Mathf::Clamp(a, min, max);
        }

        [[nodiscard]] constexpr ColorT Clamped(T min = 0, T max = 1) const
        {
            return ColorT {
                Mathf::Clamp(r, min, max),
                Mathf::Clamp(g, min, max),
                Mathf::Clamp(b, min, max),
                Mathf::Clamp(a, min, max)
            };
        }

        [[nodiscard]] constexpr T MaxColorComponent() const { return Mathf::Max(r, g, b); }

        [[nodiscard]] std::string ToString() const;

        [[nodiscard]] std::string ToHtmlString() const
        {
            return fmt::format(
                "#{:02X}{:02X}{:02X}{:02X}",
                static_cast<uint32>(Mathf::Clamp(r * 255)),
                static_cast<uint32>(Mathf::Clamp(g * 255)),
                static_cast<uint32>(Mathf::Clamp(b * 255)),
                static_cast<uint32>(Mathf::Clamp(a * 255))
            );
        }

        [[nodiscard]] static ColorT HsvToRgb(T h, T s, T v, T a = 1)
        {
            if (s == 0)
            {
                return ColorT { v, a };
            }
            else
            {
                h = Mathf::IsEqualApproximately(h, T(360)) ? 0 : h / T(60);

                int32 i = static_cast<int32>(Mathf::Floor(h));

                T f = h - i;
                T p = v * (T(1) - s);
                T q = v * (T(1) - s * f);
                T t = v * (T(1) - s * (T(1) - f));

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
            T max = Mathf::Max(color.r, color.g, color.b);
            T min = Mathf::Min(color.r, color.g, color.b);

            v = max;
            T delta = max - min;

            if (delta < Mathf::Epsilon)
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

        [[nodiscard]] static ColorT LerpUnclamped(ColorT from, ColorT to, T t)
        {
            return from + (to - from) * t;
        }

        [[nodiscard]] static ColorT Lerp(ColorT from, ColorT to, T t)
        {
            return LerpUnclamped(from, to, Mathf::Clamp(t));
        }

        [[nodiscard]] static constexpr bool IsEqualApproximately(ColorT a, ColorT b, T tolerance = Mathf::Epsilon)
        {
            return Mathf::IsEqualApproximately(a.r, b.r, tolerance) && Mathf::IsEqualApproximately(a.g, b.g, tolerance) && Mathf::IsEqualApproximately(a.b, b.b, tolerance) && Mathf::IsEqualApproximately(a.a, b.a, tolerance);
        }

        [[nodiscard]] static constexpr ColorT Min(ColorT a, ColorT b)
        {
            return ColorT {
                Mathf::Min(a.r, b.r),
                Mathf::Min(a.g, b.g),
                Mathf::Min(a.b, b.b),
                Mathf::Min(a.a, b.a)
            };
        }

        [[nodiscard]] static constexpr ColorT Min(ColorT a, ColorT b, ColorT c)
        {
            return Min(Min(a, b), c);
        }

        [[nodiscard]] static constexpr ColorT Max(ColorT a, ColorT b)
        {
            return ColorT {
                Mathf::Max(a.r, b.r),
                Mathf::Max(a.g, b.g),
                Mathf::Max(a.b, b.b),
                Mathf::Max(a.a, b.a)
            };
        }

        [[nodiscard]] static constexpr ColorT Max(ColorT a, ColorT b, ColorT c)
        {
            return Max(Max(a, b), c);
        }

        [[nodiscard]] static constexpr ColorT Black() { return ColorT(0.0); }
        [[nodiscard]] static constexpr ColorT White() { return ColorT(1.0); }
        [[nodiscard]] static constexpr ColorT Red() { return ColorT(1.0, 0.0, 0.0); }
        [[nodiscard]] static constexpr ColorT Green() { return ColorT(0.0, 1.0, 0.0); }
        [[nodiscard]] static constexpr ColorT Blue() { return ColorT(0.0, 0.0, 1.0); }

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
            BE_DEBUG_CHECK(index >= 0 && index < 4);
            return data[index];
        }

        [[nodiscard]] constexpr T operator[](int32 index) const
        {
            BE_DEBUG_CHECK(index >= 0 && index < 4);
            return data[index];
        }
    };

    using ColorF = ColorT<float>;
    using ColorD = ColorT<double>;
    using Color = ColorF;
} // namespace ByteEngine::Math

namespace fmt
{
    template <std::floating_point T>
    struct formatter<ByteEngine::Math::ColorT<T>>
    {
        constexpr auto parse(const format_parse_context& ctx) const
        {
            return ctx.begin();
        }

        auto format(const ByteEngine::Math::DegreeT<T>& value, const format_context& ctx) const
        {
            return format_to(ctx.out(), "Color({:.3f}, {:.3f}, {:.3f}, {:.3f})", value.r, value.b, value.g, value.a);
        }
    };
}

namespace ByteEngine::Math
{
    template <std::floating_point T>
    std::string ColorT<T>::ToString() const
    {
        return fmt::format("{}", *this);
    }
}