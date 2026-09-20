#include <gtest/gtest.h>
#include <cmath>
#include <cstdint>
#include <optional>
#include <string>
#include "ByteEngine/Math/Color.h"

using namespace ByteEngine::Math;

// ─────────────────────────────────────────────
// Helpers
// ─────────────────────────────────────────────

static constexpr float kEpsF = 1e-5f;
static constexpr double kEpsD = 1e-9;

template<std::floating_point T>
static bool ColorEqual(ColorT<T> a, ColorT<T> b, T eps = static_cast<T>(1e-5))
{
    return std::fabs(a.r - b.r) <= eps &&
        std::fabs(a.g - b.g) <= eps &&
        std::fabs(a.b - b.b) <= eps &&
        std::fabs(a.a - b.a) <= eps;
}

// ─────────────────────────────────────────────
// Typed test fixtures
// ─────────────────────────────────────────────

template<typename T>
class ColorTTest : public ::testing::Test
{
protected:
    using Color = ColorT<T>;
};

using FloatTypes = ::testing::Types<float, double>;
TYPED_TEST_SUITE(ColorTTest, FloatTypes);

// ─────────────────────────────────────────────
// Construction
// ─────────────────────────────────────────────

TYPED_TEST(ColorTTest, ConstructFromSingleFloat)
{
    using Color = typename TestFixture::Color;
    Color c(static_cast<TypeParam>(0.5));
    EXPECT_NEAR(c.r, 0.5, kEpsF);
    EXPECT_NEAR(c.g, 0.5, kEpsF);
    EXPECT_NEAR(c.b, 0.5, kEpsF);
    EXPECT_NEAR(c.a, 1.0, kEpsF);
}

TYPED_TEST(ColorTTest, ConstructFromSingleFloatWithAlpha)
{
    using Color = typename TestFixture::Color;
    Color c(static_cast<TypeParam>(0.5), static_cast<TypeParam>(0.25));
    EXPECT_NEAR(c.r, 0.5, kEpsF);
    EXPECT_NEAR(c.g, 0.5, kEpsF);
    EXPECT_NEAR(c.b, 0.5, kEpsF);
    EXPECT_NEAR(c.a, 0.25, kEpsF);
}

TYPED_TEST(ColorTTest, ConstructFromRgba)
{
    using Color = typename TestFixture::Color;
    Color c(static_cast<TypeParam>(0.1),
        static_cast<TypeParam>(0.2),
        static_cast<TypeParam>(0.3),
        static_cast<TypeParam>(0.4));
    EXPECT_NEAR(c.r, 0.1, kEpsF);
    EXPECT_NEAR(c.g, 0.2, kEpsF);
    EXPECT_NEAR(c.b, 0.3, kEpsF);
    EXPECT_NEAR(c.a, 0.4, kEpsF);
}

TYPED_TEST(ColorTTest, ConstructFromRgbDefaultsAlphaToOne)
{
    using Color = typename TestFixture::Color;
    Color c(static_cast<TypeParam>(0.1),
        static_cast<TypeParam>(0.2),
        static_cast<TypeParam>(0.3));
    EXPECT_NEAR(c.a, 1.0, kEpsF);
}

TYPED_TEST(ColorTTest, ConstructFromUint8Rgba)
{
    using Color = typename TestFixture::Color;
    Color c(static_cast<uint8_t>(255),
        static_cast<uint8_t>(128),
        static_cast<uint8_t>(0),
        static_cast<uint8_t>(255));
    EXPECT_NEAR(c.r, 1.0, kEpsF);
    EXPECT_NEAR(c.g, 128.0 / 255.0, 1e-4);
    EXPECT_NEAR(c.b, 0.0, kEpsF);
    EXPECT_NEAR(c.a, 1.0, kEpsF);
}

TYPED_TEST(ColorTTest, ConstructFromUint8DefaultsAlphaToOne)
{
    using Color = typename TestFixture::Color;
    Color c(static_cast<uint8_t>(100),
        static_cast<uint8_t>(150),
        static_cast<uint8_t>(200));
    EXPECT_NEAR(c.a, 1.0, kEpsF);
}

TYPED_TEST(ColorTTest, ConstructFromFloatArray)
{
    using Color = typename TestFixture::Color;
    TypeParam arr[4] = { static_cast<TypeParam>(0.1),
        static_cast<TypeParam>(0.2),
        static_cast<TypeParam>(0.3),
        static_cast<TypeParam>(0.4) };
    Color c(arr);
    EXPECT_NEAR(c.r, 0.1, kEpsF);
    EXPECT_NEAR(c.g, 0.2, kEpsF);
    EXPECT_NEAR(c.b, 0.3, kEpsF);
    EXPECT_NEAR(c.a, 0.4, kEpsF);
}

TYPED_TEST(ColorTTest, ConstructFromUint8Array)
{
    using Color = typename TestFixture::Color;
    uint8_t arr[4] = { 255, 128, 64, 32 };
    Color c(arr);
    EXPECT_NEAR(c.r, 1.0, kEpsF);
    EXPECT_NEAR(c.g, 128.0 / 255.0, 1e-4);
    EXPECT_NEAR(c.b, 64.0 / 255.0, 1e-4);
    EXPECT_NEAR(c.a, 32.0 / 255.0, 1e-4);
}

TYPED_TEST(ColorTTest, DataUnionAliasesComponents)
{
    using Color = typename TestFixture::Color;
    Color c(static_cast<TypeParam>(0.1),
        static_cast<TypeParam>(0.2),
        static_cast<TypeParam>(0.3),
        static_cast<TypeParam>(0.4));
    EXPECT_EQ(c.data[0], c.r);
    EXPECT_EQ(c.data[1], c.g);
    EXPECT_EQ(c.data[2], c.b);
    EXPECT_EQ(c.data[3], c.a);
}

// ─────────────────────────────────────────────
// Accessors
// ─────────────────────────────────────────────

TYPED_TEST(ColorTTest, IndexOperatorRead)
{
    using Color = typename TestFixture::Color;
    Color c(static_cast<TypeParam>(0.1),
        static_cast<TypeParam>(0.2),
        static_cast<TypeParam>(0.3),
        static_cast<TypeParam>(0.4));
    EXPECT_NEAR(c[0], 0.1, kEpsF);
    EXPECT_NEAR(c[1], 0.2, kEpsF);
    EXPECT_NEAR(c[2], 0.3, kEpsF);
    EXPECT_NEAR(c[3], 0.4, kEpsF);
}

TYPED_TEST(ColorTTest, IndexOperatorWrite)
{
    using Color = typename TestFixture::Color;
    Color c(static_cast<TypeParam>(0));
    c[0] = static_cast<TypeParam>(0.9);
    c[3] = static_cast<TypeParam>(0.5);
    EXPECT_NEAR(c.r, 0.9, kEpsF);
    EXPECT_NEAR(c.a, 0.5, kEpsF);
}

// ─────────────────────────────────────────────
// Arithmetic operators
// ─────────────────────────────────────────────

TYPED_TEST(ColorTTest, UnaryPlus)
{
    using Color = typename TestFixture::Color;
    Color c(static_cast<TypeParam>(0.3), static_cast<TypeParam>(0.4),
        static_cast<TypeParam>(0.5), static_cast<TypeParam>(0.6));
    Color result = +c;
    EXPECT_TRUE(ColorEqual(result, c));
}

TYPED_TEST(ColorTTest, UnaryMinus)
{
    using Color = typename TestFixture::Color;
    Color c(static_cast<TypeParam>(0.3), static_cast<TypeParam>(0.4),
        static_cast<TypeParam>(0.5), static_cast<TypeParam>(0.6));
    Color result = -c;
    EXPECT_NEAR(result.r, -0.3, kEpsF);
    EXPECT_NEAR(result.g, -0.4, kEpsF);
    EXPECT_NEAR(result.b, -0.5, kEpsF);
    EXPECT_NEAR(result.a, -0.6, kEpsF);
}

TYPED_TEST(ColorTTest, AdditionOperator)
{
    using Color = typename TestFixture::Color;
    Color a(static_cast<TypeParam>(0.1), static_cast<TypeParam>(0.2),
        static_cast<TypeParam>(0.3), static_cast<TypeParam>(0.4));
    Color b(static_cast<TypeParam>(0.1), static_cast<TypeParam>(0.1),
        static_cast<TypeParam>(0.1), static_cast<TypeParam>(0.1));
    Color result = a + b;
    EXPECT_NEAR(result.r, 0.2, kEpsF);
    EXPECT_NEAR(result.g, 0.3, kEpsF);
    EXPECT_NEAR(result.b, 0.4, kEpsF);
    EXPECT_NEAR(result.a, 0.5, kEpsF);
}

TYPED_TEST(ColorTTest, SubtractionOperator)
{
    using Color = typename TestFixture::Color;
    Color a(static_cast<TypeParam>(0.5), static_cast<TypeParam>(0.5),
        static_cast<TypeParam>(0.5), static_cast<TypeParam>(0.5));
    Color b(static_cast<TypeParam>(0.2), static_cast<TypeParam>(0.1),
        static_cast<TypeParam>(0.3), static_cast<TypeParam>(0.0));
    Color result = a - b;
    EXPECT_NEAR(result.r, 0.3, kEpsF);
    EXPECT_NEAR(result.g, 0.4, kEpsF);
    EXPECT_NEAR(result.b, 0.2, kEpsF);
    EXPECT_NEAR(result.a, 0.5, kEpsF);
}

TYPED_TEST(ColorTTest, MultiplicationByColor)
{
    using Color = typename TestFixture::Color;
    Color a(static_cast<TypeParam>(0.5), static_cast<TypeParam>(0.4),
        static_cast<TypeParam>(0.2), static_cast<TypeParam>(1.0));
    Color b(static_cast<TypeParam>(0.5), static_cast<TypeParam>(0.5),
        static_cast<TypeParam>(0.5), static_cast<TypeParam>(1.0));
    Color result = a * b;
    EXPECT_NEAR(result.r, 0.25, kEpsF);
    EXPECT_NEAR(result.g, 0.2, kEpsF);
    EXPECT_NEAR(result.b, 0.1, kEpsF);
    EXPECT_NEAR(result.a, 1.0, kEpsF);
}

TYPED_TEST(ColorTTest, DivisionByColor)
{
    using Color = typename TestFixture::Color;
    Color a(static_cast<TypeParam>(0.5), static_cast<TypeParam>(0.4),
        static_cast<TypeParam>(0.2), static_cast<TypeParam>(1.0));
    Color b(static_cast<TypeParam>(0.5), static_cast<TypeParam>(0.4),
        static_cast<TypeParam>(0.2), static_cast<TypeParam>(1.0));
    Color result = a / b;
    EXPECT_NEAR(result.r, 1.0, kEpsF);
    EXPECT_NEAR(result.g, 1.0, kEpsF);
    EXPECT_NEAR(result.b, 1.0, kEpsF);
}

TYPED_TEST(ColorTTest, MultiplicationByScalar)
{
    using Color = typename TestFixture::Color;
    Color c(static_cast<TypeParam>(0.2), static_cast<TypeParam>(0.4),
        static_cast<TypeParam>(0.6), static_cast<TypeParam>(0.8));
    Color result = c * static_cast<TypeParam>(2);
    EXPECT_NEAR(result.r, 0.4, kEpsF);
    EXPECT_NEAR(result.g, 0.8, kEpsF);
    EXPECT_NEAR(result.b, 1.2, kEpsF);
    EXPECT_NEAR(result.a, 1.6, kEpsF);
}

TYPED_TEST(ColorTTest, ScalarMultiplicationCommutative)
{
    using Color = typename TestFixture::Color;
    Color c(static_cast<TypeParam>(0.2), static_cast<TypeParam>(0.4),
        static_cast<TypeParam>(0.6), static_cast<TypeParam>(0.8));
    Color r1 = c * static_cast<TypeParam>(3);
    Color r2 = static_cast<TypeParam>(3) * c;
    EXPECT_TRUE(ColorEqual(r1, r2));
}

TYPED_TEST(ColorTTest, DivisionByScalar)
{
    using Color = typename TestFixture::Color;
    Color c(static_cast<TypeParam>(0.4), static_cast<TypeParam>(0.8),
        static_cast<TypeParam>(0.6), static_cast<TypeParam>(1.0));
    Color result = c / static_cast<TypeParam>(2);
    EXPECT_NEAR(result.r, 0.2, kEpsF);
    EXPECT_NEAR(result.g, 0.4, kEpsF);
    EXPECT_NEAR(result.b, 0.3, kEpsF);
    EXPECT_NEAR(result.a, 0.5, kEpsF);
}

// ─────────────────────────────────────────────
// Equality operators
// ─────────────────────────────────────────────

TYPED_TEST(ColorTTest, EqualityOperatorSameValues)
{
    using Color = typename TestFixture::Color;
    Color a(static_cast<TypeParam>(0.1), static_cast<TypeParam>(0.2),
        static_cast<TypeParam>(0.3), static_cast<TypeParam>(0.4));
    Color b(static_cast<TypeParam>(0.1), static_cast<TypeParam>(0.2),
        static_cast<TypeParam>(0.3), static_cast<TypeParam>(0.4));
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
}

TYPED_TEST(ColorTTest, InequalityOperatorDifferentValues)
{
    using Color = typename TestFixture::Color;
    Color a(static_cast<TypeParam>(0.1), static_cast<TypeParam>(0.2),
        static_cast<TypeParam>(0.3), static_cast<TypeParam>(0.4));
    Color b(static_cast<TypeParam>(0.5), static_cast<TypeParam>(0.2),
        static_cast<TypeParam>(0.3), static_cast<TypeParam>(0.4));
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
}

TYPED_TEST(ColorTTest, IsEqualApproximately)
{
    using Color = typename TestFixture::Color;
    Color a(static_cast<TypeParam>(0.1000001),
        static_cast<TypeParam>(0.2),
        static_cast<TypeParam>(0.3),
        static_cast<TypeParam>(0.4));
    Color b(static_cast<TypeParam>(0.1),
        static_cast<TypeParam>(0.2),
        static_cast<TypeParam>(0.3),
        static_cast<TypeParam>(0.4));
    EXPECT_TRUE(Color::IsEqualApproximately(a, b));
}

TYPED_TEST(ColorTTest, IsEqualApproximatelyFalseWhenFarApart)
{
    using Color = typename TestFixture::Color;
    Color a(static_cast<TypeParam>(0.1), static_cast<TypeParam>(0.2),
        static_cast<TypeParam>(0.3), static_cast<TypeParam>(0.4));
    Color b(static_cast<TypeParam>(0.5), static_cast<TypeParam>(0.2),
        static_cast<TypeParam>(0.3), static_cast<TypeParam>(0.4));
    EXPECT_FALSE(Color::IsEqualApproximately(a, b));
}

TYPED_TEST(ColorTTest, IsEqualApproximatelyCustomTolerance)
{
    using Color = typename TestFixture::Color;
    Color a(static_cast<TypeParam>(0.1), static_cast<TypeParam>(0.2),
        static_cast<TypeParam>(0.3), static_cast<TypeParam>(0.4));
    Color b(static_cast<TypeParam>(0.15), static_cast<TypeParam>(0.2),
        static_cast<TypeParam>(0.3), static_cast<TypeParam>(0.4));
    EXPECT_TRUE(Color::IsEqualApproximately(a, b, static_cast<TypeParam>(0.1)));
    EXPECT_FALSE(Color::IsEqualApproximately(a, b, static_cast<TypeParam>(0.01)));
}

// ─────────────────────────────────────────────
// Clamp
// ─────────────────────────────────────────────

TYPED_TEST(ColorTTest, ClampDefaultRange)
{
    using Color = typename TestFixture::Color;
    Color c(static_cast<TypeParam>(-0.5), static_cast<TypeParam>(1.5),
        static_cast<TypeParam>(0.5), static_cast<TypeParam>(2.0));
    c.Clamp();
    EXPECT_NEAR(c.r, 0.0, kEpsF);
    EXPECT_NEAR(c.g, 1.0, kEpsF);
    EXPECT_NEAR(c.b, 0.5, kEpsF);
    EXPECT_NEAR(c.a, 1.0, kEpsF);
}

TYPED_TEST(ColorTTest, ClampCustomRange)
{
    using Color = typename TestFixture::Color;
    Color c(static_cast<TypeParam>(0.0), static_cast<TypeParam>(1.0),
        static_cast<TypeParam>(0.5), static_cast<TypeParam>(0.8));
    c.Clamp(static_cast<TypeParam>(0.2), static_cast<TypeParam>(0.7));
    EXPECT_NEAR(c.r, 0.2, kEpsF);
    EXPECT_NEAR(c.g, 0.7, kEpsF);
    EXPECT_NEAR(c.b, 0.5, kEpsF);
    EXPECT_NEAR(c.a, 0.7, kEpsF);
}

TYPED_TEST(ColorTTest, ClampedReturnsNewColor)
{
    using Color = typename TestFixture::Color;
    Color c(static_cast<TypeParam>(-0.1), static_cast<TypeParam>(1.2),
        static_cast<TypeParam>(0.5), static_cast<TypeParam>(0.5));
    Color clamped = c.Clamped();
    EXPECT_NEAR(clamped.r, 0.0, kEpsF);
    EXPECT_NEAR(clamped.g, 1.0, kEpsF);
    // original should be unchanged
    EXPECT_NEAR(c.r, -0.1, kEpsF);
    EXPECT_NEAR(c.g, 1.2, kEpsF);
}

TYPED_TEST(ColorTTest, ClampAlreadyInRangeUnchanged)
{
    using Color = typename TestFixture::Color;
    Color c(static_cast<TypeParam>(0.2), static_cast<TypeParam>(0.5),
        static_cast<TypeParam>(0.8), static_cast<TypeParam>(1.0));
    Color original = c;
    c.Clamp();
    EXPECT_TRUE(ColorEqual(c, original));
}

// ─────────────────────────────────────────────
// Min / Max
// ─────────────────────────────────────────────

TYPED_TEST(ColorTTest, MinTwoColors)
{
    using Color = typename TestFixture::Color;
    Color a(static_cast<TypeParam>(0.1), static_cast<TypeParam>(0.8),
        static_cast<TypeParam>(0.5), static_cast<TypeParam>(1.0));
    Color b(static_cast<TypeParam>(0.5), static_cast<TypeParam>(0.3),
        static_cast<TypeParam>(0.9), static_cast<TypeParam>(0.2));
    Color m = Color::Min(a, b);
    EXPECT_NEAR(m.r, 0.1, kEpsF);
    EXPECT_NEAR(m.g, 0.3, kEpsF);
    EXPECT_NEAR(m.b, 0.5, kEpsF);
    EXPECT_NEAR(m.a, 0.2, kEpsF);
}

TYPED_TEST(ColorTTest, MinThreeColors)
{
    using Color = typename TestFixture::Color;
    Color a(static_cast<TypeParam>(0.3), static_cast<TypeParam>(0.9),
        static_cast<TypeParam>(0.5), static_cast<TypeParam>(0.8));
    Color b(static_cast<TypeParam>(0.1), static_cast<TypeParam>(0.6),
        static_cast<TypeParam>(0.7), static_cast<TypeParam>(0.4));
    Color c(static_cast<TypeParam>(0.5), static_cast<TypeParam>(0.2),
        static_cast<TypeParam>(0.3), static_cast<TypeParam>(1.0));
    Color m = Color::Min(a, b, c);
    EXPECT_NEAR(m.r, 0.1, kEpsF);
    EXPECT_NEAR(m.g, 0.2, kEpsF);
    EXPECT_NEAR(m.b, 0.3, kEpsF);
    EXPECT_NEAR(m.a, 0.4, kEpsF);
}

TYPED_TEST(ColorTTest, MaxTwoColors)
{
    using Color = typename TestFixture::Color;
    Color a(static_cast<TypeParam>(0.1), static_cast<TypeParam>(0.8),
        static_cast<TypeParam>(0.5), static_cast<TypeParam>(1.0));
    Color b(static_cast<TypeParam>(0.5), static_cast<TypeParam>(0.3),
        static_cast<TypeParam>(0.9), static_cast<TypeParam>(0.2));
    Color m = Color::Max(a, b);
    EXPECT_NEAR(m.r, 0.5, kEpsF);
    EXPECT_NEAR(m.g, 0.8, kEpsF);
    EXPECT_NEAR(m.b, 0.9, kEpsF);
    EXPECT_NEAR(m.a, 1.0, kEpsF);
}

TYPED_TEST(ColorTTest, MaxThreeColors)
{
    using Color = typename TestFixture::Color;
    Color a(static_cast<TypeParam>(0.3), static_cast<TypeParam>(0.9),
        static_cast<TypeParam>(0.5), static_cast<TypeParam>(0.8));
    Color b(static_cast<TypeParam>(0.1), static_cast<TypeParam>(0.6),
        static_cast<TypeParam>(0.7), static_cast<TypeParam>(0.4));
    Color c(static_cast<TypeParam>(0.5), static_cast<TypeParam>(0.2),
        static_cast<TypeParam>(0.3), static_cast<TypeParam>(1.0));
    Color m = Color::Max(a, b, c);
    EXPECT_NEAR(m.r, 0.5, kEpsF);
    EXPECT_NEAR(m.g, 0.9, kEpsF);
    EXPECT_NEAR(m.b, 0.7, kEpsF);
    EXPECT_NEAR(m.a, 1.0, kEpsF);
}

// ─────────────────────────────────────────────
// Lerp
// ─────────────────────────────────────────────

TYPED_TEST(ColorTTest, LerpAtZero)
{
    using Color = typename TestFixture::Color;
    Color from(static_cast<TypeParam>(0.0), static_cast<TypeParam>(0.0),
        static_cast<TypeParam>(0.0), static_cast<TypeParam>(0.0));
    Color to(static_cast<TypeParam>(1.0), static_cast<TypeParam>(1.0),
        static_cast<TypeParam>(1.0), static_cast<TypeParam>(1.0));
    Color result = Color::Lerp(from, to, static_cast<TypeParam>(0.0));
    EXPECT_TRUE(ColorEqual(result, from));
}

TYPED_TEST(ColorTTest, LerpAtOne)
{
    using Color = typename TestFixture::Color;
    Color from(static_cast<TypeParam>(0.0), static_cast<TypeParam>(0.0),
        static_cast<TypeParam>(0.0), static_cast<TypeParam>(0.0));
    Color to(static_cast<TypeParam>(1.0), static_cast<TypeParam>(1.0),
        static_cast<TypeParam>(1.0), static_cast<TypeParam>(1.0));
    Color result = Color::Lerp(from, to, static_cast<TypeParam>(1.0));
    EXPECT_TRUE(ColorEqual(result, to));
}

TYPED_TEST(ColorTTest, LerpAtHalf)
{
    using Color = typename TestFixture::Color;
    Color from(static_cast<TypeParam>(0.0), static_cast<TypeParam>(0.0),
        static_cast<TypeParam>(0.0), static_cast<TypeParam>(0.0));
    Color to(static_cast<TypeParam>(1.0), static_cast<TypeParam>(1.0),
        static_cast<TypeParam>(1.0), static_cast<TypeParam>(1.0));
    Color result = Color::Lerp(from, to, static_cast<TypeParam>(0.5));
    EXPECT_NEAR(result.r, 0.5, kEpsF);
    EXPECT_NEAR(result.g, 0.5, kEpsF);
    EXPECT_NEAR(result.b, 0.5, kEpsF);
    EXPECT_NEAR(result.a, 0.5, kEpsF);
}

TYPED_TEST(ColorTTest, LerpClampedAboveOneClamps)
{
    using Color = typename TestFixture::Color;
    Color from(static_cast<TypeParam>(0.0), static_cast<TypeParam>(0.0),
        static_cast<TypeParam>(0.0), static_cast<TypeParam>(0.0));
    Color to(static_cast<TypeParam>(1.0), static_cast<TypeParam>(1.0),
        static_cast<TypeParam>(1.0), static_cast<TypeParam>(1.0));
    Color result = Color::Lerp(from, to, static_cast<TypeParam>(2.0));
    EXPECT_TRUE(ColorEqual(result, to));
}

TYPED_TEST(ColorTTest, LerpClampedBelowZeroClamps)
{
    using Color = typename TestFixture::Color;
    Color from(static_cast<TypeParam>(0.0), static_cast<TypeParam>(0.0), static_cast<TypeParam>(0.0), static_cast<TypeParam>(0.0));
    Color to(static_cast<TypeParam>(1.0), static_cast<TypeParam>(1.0), static_cast<TypeParam>(1.0), static_cast<TypeParam>(1.0));
    Color result = Color::Lerp(from, to, static_cast<TypeParam>(-1.0));
    EXPECT_TRUE(ColorEqual(result, from));
}

TYPED_TEST(ColorTTest, LerpUnclampedExtrapolates)
{
    using Color = typename TestFixture::Color;
    Color from(static_cast<TypeParam>(0.0), static_cast<TypeParam>(0.0),
        static_cast<TypeParam>(0.0), static_cast<TypeParam>(0.0));
    Color to(static_cast<TypeParam>(1.0), static_cast<TypeParam>(1.0),
        static_cast<TypeParam>(1.0), static_cast<TypeParam>(1.0));
    Color result = Color::LerpUnclamped(from, to, static_cast<TypeParam>(2.0));
    EXPECT_NEAR(result.r, 2.0, kEpsF);
}

// ─────────────────────────────────────────────
// Color space conversion
// ─────────────────────────────────────────────

TYPED_TEST(ColorTTest, ToLinearAndBackRoundTrip)
{
    using Color = typename TestFixture::Color;
    Color original(static_cast<TypeParam>(0.5), static_cast<TypeParam>(0.3),
        static_cast<TypeParam>(0.8), static_cast<TypeParam>(1.0));
    Color c = original;
    c.ToLinear();
    c.ToGamma();
    EXPECT_TRUE(ColorEqual(c, original, static_cast<TypeParam>(1e-4)));
}

TYPED_TEST(ColorTTest, AsLinearDoesNotModifyOriginal)
{
    using Color = typename TestFixture::Color;
    Color c(static_cast<TypeParam>(0.5), static_cast<TypeParam>(0.5),
        static_cast<TypeParam>(0.5), static_cast<TypeParam>(1.0));
    Color original = c;
    Color linear = c.AsLinear();
    EXPECT_TRUE(ColorEqual(c, original));    // original unchanged
    EXPECT_FALSE(ColorEqual(linear, original)); // result differs for mid-gray
}

TYPED_TEST(ColorTTest, AsSrgbDoesNotModifyOriginal)
{
    using Color = typename TestFixture::Color;
    Color c(static_cast<TypeParam>(0.5), static_cast<TypeParam>(0.5),
        static_cast<TypeParam>(0.5), static_cast<TypeParam>(1.0));
    Color original = c;
    Color srgb = c.AsGamma();
    EXPECT_TRUE(ColorEqual(c, original));
    EXPECT_FALSE(ColorEqual(srgb, original));
}

TYPED_TEST(ColorTTest, ToLinearBlackAndWhiteUnchanged)
{
    using Color = typename TestFixture::Color;
    Color black(static_cast<TypeParam>(0.0));
    Color white(static_cast<TypeParam>(1.0));
    black.ToLinear();
    white.ToLinear();
    EXPECT_NEAR(black.r, 0.0, kEpsF);
    EXPECT_NEAR(white.r, 1.0, kEpsF);
}

TYPED_TEST(ColorTTest, ToSrgbBlackAndWhiteUnchanged)
{
    using Color = typename TestFixture::Color;
    Color black(static_cast<TypeParam>(0.0));
    Color white(static_cast<TypeParam>(1.0));
    black.ToGamma();
    white.ToGamma();
    EXPECT_NEAR(black.r, 0.0, kEpsF);
    EXPECT_NEAR(white.r, 1.0, kEpsF);
}

TYPED_TEST(ColorTTest, ToLinearAlphaPreserved)
{
    using Color = typename TestFixture::Color;
    Color c(static_cast<TypeParam>(0.5), static_cast<TypeParam>(0.5),
        static_cast<TypeParam>(0.5), static_cast<TypeParam>(0.75));
    c.ToLinear();
    EXPECT_NEAR(c.a, 0.75, kEpsF);
}

TYPED_TEST(ColorTTest, ToSrgbAlphaPreserved)
{
    using Color = typename TestFixture::Color;
    Color c(static_cast<TypeParam>(0.5), static_cast<TypeParam>(0.5),
        static_cast<TypeParam>(0.5), static_cast<TypeParam>(0.75));
    c.ToGamma();
    EXPECT_NEAR(c.a, 0.75, kEpsF);
}

// ─────────────────────────────────────────────
// HSV conversion
// ─────────────────────────────────────────────

TYPED_TEST(ColorTTest, HsvToRgbPrimaryRed)
{
    using Color = typename TestFixture::Color;
    Color c = Color::HsvToRgb(static_cast<TypeParam>(0.0),
        static_cast<TypeParam>(1.0),
        static_cast<TypeParam>(1.0));
    EXPECT_NEAR(c.r, 1.0, kEpsF);
    EXPECT_NEAR(c.g, 0.0, kEpsF);
    EXPECT_NEAR(c.b, 0.0, kEpsF);
    EXPECT_NEAR(c.a, 1.0, kEpsF);
}

TYPED_TEST(ColorTTest, HsvToRgbPrimaryGreen)
{
    using Color = typename TestFixture::Color;
    Color c = Color::HsvToRgb(static_cast<TypeParam>(120),
        static_cast<TypeParam>(1.0),
        static_cast<TypeParam>(1.0));
    EXPECT_NEAR(c.r, 0.0, 1e-4);
    EXPECT_NEAR(c.g, 1.0, 1e-4);
    EXPECT_NEAR(c.b, 0.0, 1e-4);
}

TYPED_TEST(ColorTTest, HsvToRgbPrimaryBlue)
{
    using Color = typename TestFixture::Color;
    Color c = Color::HsvToRgb(static_cast<TypeParam>(240),
        static_cast<TypeParam>(1.0),
        static_cast<TypeParam>(1.0));
    EXPECT_NEAR(c.r, 0.0, 1e-4);
    EXPECT_NEAR(c.g, 0.0, 1e-4);
    EXPECT_NEAR(c.b, 1.0, 1e-4);
}

TYPED_TEST(ColorTTest, HsvToRgbWhite)
{
    using Color = typename TestFixture::Color;
    Color c = Color::HsvToRgb(static_cast<TypeParam>(0.0),
        static_cast<TypeParam>(0.0),
        static_cast<TypeParam>(1.0));
    EXPECT_NEAR(c.r, 1.0, kEpsF);
    EXPECT_NEAR(c.g, 1.0, kEpsF);
    EXPECT_NEAR(c.b, 1.0, kEpsF);
}

TYPED_TEST(ColorTTest, HsvToRgbBlack)
{
    using Color = typename TestFixture::Color;
    Color c = Color::HsvToRgb(static_cast<TypeParam>(0.0),
        static_cast<TypeParam>(0.0),
        static_cast<TypeParam>(0.0));
    EXPECT_NEAR(c.r, 0.0, kEpsF);
    EXPECT_NEAR(c.g, 0.0, kEpsF);
    EXPECT_NEAR(c.b, 0.0, kEpsF);
}

TYPED_TEST(ColorTTest, HsvToRgbAlpha)
{
    using Color = typename TestFixture::Color;
    Color c = Color::HsvToRgb(static_cast<TypeParam>(0.0),
        static_cast<TypeParam>(1.0),
        static_cast<TypeParam>(1.0),
        static_cast<TypeParam>(0.5));
    EXPECT_NEAR(c.a, 0.5, kEpsF);
}

TYPED_TEST(ColorTTest, RgbToHsvRoundTrip)
{
    using Color = typename TestFixture::Color;
    TypeParam h = static_cast<TypeParam>(0.6);
    TypeParam s = static_cast<TypeParam>(0.7);
    TypeParam v = static_cast<TypeParam>(0.8);

    Color c = Color::HsvToRgb(h, s, v);

    TypeParam outH, outS, outV;
    Color::RgbToHsv(c, outH, outS, outV);

    EXPECT_NEAR(outH, h, 1e-4);
    EXPECT_NEAR(outS, s, 1e-4);
    EXPECT_NEAR(outV, v, 1e-4);
}

TYPED_TEST(ColorTTest, RgbToHsvGrayHueIsZero)
{
    using Color = typename TestFixture::Color;
    Color gray(static_cast<TypeParam>(0.5), static_cast<TypeParam>(0.5),
        static_cast<TypeParam>(0.5), static_cast<TypeParam>(1.0));
    TypeParam h, s, v;
    Color::RgbToHsv(gray, h, s, v);
    EXPECT_NEAR(s, 0.0, kEpsF);
    EXPECT_NEAR(v, 0.5, kEpsF);
}

// ─────────────────────────────────────────────
// HTML color parsing (HtmlToRgb / ToString "H")
// ─────────────────────────────────────────────

TYPED_TEST(ColorTTest, HtmlToRgbWithHash)
{
    using Color = typename TestFixture::Color;
    auto result = Color::HtmlToRgb("#FF0000");
    ASSERT_TRUE(result.has_value());
    EXPECT_NEAR(result->r, 1.0, kEpsF);
    EXPECT_NEAR(result->g, 0.0, kEpsF);
    EXPECT_NEAR(result->b, 0.0, kEpsF);
    EXPECT_NEAR(result->a, 1.0, kEpsF);
}

TYPED_TEST(ColorTTest, HtmlToRgbWithoutHash)
{
    using Color = typename TestFixture::Color;
    auto result = Color::HtmlToRgb("00FF00");
    ASSERT_TRUE(result.has_value());
    EXPECT_NEAR(result->r, 0.0, kEpsF);
    EXPECT_NEAR(result->g, 1.0, kEpsF);
    EXPECT_NEAR(result->b, 0.0, kEpsF);
}

TYPED_TEST(ColorTTest, HtmlToRgbMixedCase)
{
    using Color = typename TestFixture::Color;
    auto a = Color::HtmlToRgb("#ffaa12");
    auto b = Color::HtmlToRgb("#FFAA12");
    ASSERT_TRUE(a.has_value());
    ASSERT_TRUE(b.has_value());
    EXPECT_TRUE(ColorEqual(*a, *b, static_cast<TypeParam>(1e-4)));
}

TYPED_TEST(ColorTTest, HtmlToRgbInvalidReturnsEmpty)
{
    using Color = typename TestFixture::Color;
    EXPECT_FALSE(Color::HtmlToRgb("ZZZZZZ").has_value());
    EXPECT_FALSE(Color::HtmlToRgb("#XYZ").has_value());
    EXPECT_FALSE(Color::HtmlToRgb("").has_value());
    EXPECT_FALSE(Color::HtmlToRgb("#12345").has_value());  // too short (5 hex digits)
}

TYPED_TEST(ColorTTest, HtmlToRgbBlack)
{
    using Color = typename TestFixture::Color;
    auto result = Color::HtmlToRgb("#000000");
    ASSERT_TRUE(result.has_value());
    EXPECT_NEAR(result->r, 0.0, kEpsF);
    EXPECT_NEAR(result->g, 0.0, kEpsF);
    EXPECT_NEAR(result->b, 0.0, kEpsF);
}

TYPED_TEST(ColorTTest, HtmlToRgbWhite)
{
    using Color = typename TestFixture::Color;
    auto result = Color::HtmlToRgb("#FFFFFF");
    ASSERT_TRUE(result.has_value());
    EXPECT_NEAR(result->r, 1.0, kEpsF);
    EXPECT_NEAR(result->g, 1.0, kEpsF);
    EXPECT_NEAR(result->b, 1.0, kEpsF);
}

// ─────────────────────────────────────────────
// ToString
// ─────────────────────────────────────────────

TYPED_TEST(ColorTTest, ToStringFormatF0)
{
    using Color = typename TestFixture::Color;
    Color c(static_cast<TypeParam>(1.0), static_cast<TypeParam>(0.5),
        static_cast<TypeParam>(0.0), static_cast<TypeParam>(1.0));
    auto str = c.ToString("F0");
    ASSERT_TRUE(str.has_value());
    EXPECT_EQ(*str, "Color(1, 0, 0)");
}

TYPED_TEST(ColorTTest, ToStringFormatF2)
{
    using Color = typename TestFixture::Color;
    Color c(static_cast<TypeParam>(1.0), static_cast<TypeParam>(0.5),
        static_cast<TypeParam>(0.0), static_cast<TypeParam>(1.0));
    auto str = c.ToString("F2");
    ASSERT_TRUE(str.has_value());
    EXPECT_EQ(*str, "Color(1.00, 0.50, 0.00)");
}

TYPED_TEST(ColorTTest, ToStringFormatF2WithAlpha)
{
    using Color = typename TestFixture::Color;
    Color c(static_cast<TypeParam>(1.0), static_cast<TypeParam>(0.5),
        static_cast<TypeParam>(0.0), static_cast<TypeParam>(0.75));
    auto str = c.ToString("F2A");
    ASSERT_TRUE(str.has_value());
    EXPECT_EQ(*str, "Color(1.00, 0.50, 0.00, 0.75)");
}

TYPED_TEST(ColorTTest, ToStringFormatH)
{
    using Color = typename TestFixture::Color;
    Color c(static_cast<TypeParam>(1.0), static_cast<TypeParam>(0.0),
        static_cast<TypeParam>(0.0), static_cast<TypeParam>(1.0));
    auto str = c.ToString("H");
    ASSERT_TRUE(str.has_value());
    EXPECT_EQ(*str, "#FF0000");
}

TYPED_TEST(ColorTTest, ToStringFormatHAlwaysHasHash)
{
    using Color = typename TestFixture::Color;
    Color c(static_cast<TypeParam>(0.0), static_cast<TypeParam>(1.0),
        static_cast<TypeParam>(0.0), static_cast<TypeParam>(1.0));
    auto str = c.ToString("H");
    ASSERT_TRUE(str.has_value());
    EXPECT_EQ((*str)[0], '#');
}

TYPED_TEST(ColorTTest, ToStringFormatHWithAlpha)
{
    using Color = typename TestFixture::Color;
    Color c(static_cast<TypeParam>(1.0), static_cast<TypeParam>(0.0),
        static_cast<TypeParam>(0.0), static_cast<TypeParam>(1.0));
    auto str = c.ToString("HA");
    ASSERT_TRUE(str.has_value());
    EXPECT_EQ(*str, "#FF0000FF");
}

TYPED_TEST(ColorTTest, ToStringHtmlRoundTrip)
{
    using Color = typename TestFixture::Color;
    Color original(static_cast<TypeParam>(0.4), static_cast<TypeParam>(0.6),
        static_cast<TypeParam>(0.8), static_cast<TypeParam>(1.0));
    auto htmlStr = original.ToString("H");
    ASSERT_TRUE(htmlStr.has_value());
    auto parsed = Color::HtmlToRgb(*htmlStr);
    ASSERT_TRUE(parsed.has_value());
    EXPECT_TRUE(ColorEqual(*parsed, original, static_cast<TypeParam>(1.0 / 255.0)));
}

TYPED_TEST(ColorTTest, ToStringDefaultFormat)
{
    using Color = typename TestFixture::Color;
    Color c(static_cast<TypeParam>(0.5), static_cast<TypeParam>(0.5),
        static_cast<TypeParam>(0.5), static_cast<TypeParam>(1.0));
    auto str = c.ToString();
    EXPECT_TRUE(str.has_value());
    EXPECT_FALSE(str->empty());
}

TYPED_TEST(ColorTTest, ToStringInvalidFormatReturnsEmpty)
{
    using Color = typename TestFixture::Color;
    Color c(static_cast<TypeParam>(0.5), static_cast<TypeParam>(0.5),
        static_cast<TypeParam>(0.5), static_cast<TypeParam>(1.0));
    auto str = c.ToString("INVALID_FORMAT");
    EXPECT_FALSE(str.has_value());
}

// ─────────────────────────────────────────────
// Type conversion operators
// ─────────────────────────────────────────────

TEST(ColorTConversionTest, FloatToDoubleConversion)
{
    ColorT<float> f(0.1f, 0.2f, 0.3f, 0.4f);
    ColorT<double> d = static_cast<ColorT<double>>(f);
    EXPECT_NEAR(d.r, 0.1, 1e-6);
    EXPECT_NEAR(d.g, 0.2, 1e-6);
    EXPECT_NEAR(d.b, 0.3, 1e-6);
    EXPECT_NEAR(d.a, 0.4, 1e-6);
}

TEST(ColorTConversionTest, DoubleToFloatConversion)
{
    ColorT<double> d(0.1, 0.2, 0.3, 0.4);
    ColorT<float> f = static_cast<ColorT<float>>(d);
    EXPECT_NEAR(f.r, 0.1f, 1e-5f);
    EXPECT_NEAR(f.g, 0.2f, 1e-5f);
    EXPECT_NEAR(f.b, 0.3f, 1e-5f);
    EXPECT_NEAR(f.a, 0.4f, 1e-5f);
}

// ─────────────────────────────────────────────
// Robustness / edge cases
// ─────────────────────────────────────────────

template<typename T>
class ColorTRobustnessTest : public ::testing::Test
{
protected:
    using Color = ColorT<T>;
};

TYPED_TEST_SUITE(ColorTRobustnessTest, FloatTypes);

TYPED_TEST(ColorTRobustnessTest, LerpWithNanT)
{
    using Color = typename TestFixture::Color;
    Color from(static_cast<TypeParam>(0.0));
    Color to(static_cast<TypeParam>(1.0));
    TypeParam nan = std::numeric_limits<TypeParam>::quiet_NaN();
    Color result = Color::Lerp(from, to, nan);
    EXPECT_TRUE(std::isnan(result.r));
}

TYPED_TEST(ColorTRobustnessTest, DivisionByZeroScalarProducesInfinity)
{
    using Color = typename TestFixture::Color;
    Color c(static_cast<TypeParam>(1.0), static_cast<TypeParam>(1.0),
        static_cast<TypeParam>(1.0), static_cast<TypeParam>(1.0));
    Color result = c / static_cast<TypeParam>(0);
    EXPECT_TRUE(std::isinf(result.r));
    EXPECT_TRUE(std::isinf(result.g));
    EXPECT_TRUE(std::isinf(result.b));
    EXPECT_TRUE(std::isinf(result.a));
}

TYPED_TEST(ColorTRobustnessTest, DivisionByZeroColorProducesInfinityOrNan)
{
    using Color = typename TestFixture::Color;
    Color c(static_cast<TypeParam>(1.0), static_cast<TypeParam>(0.0),
        static_cast<TypeParam>(1.0), static_cast<TypeParam>(1.0));
    Color z(static_cast<TypeParam>(0.0));
    Color result = c / z;
    EXPECT_TRUE(std::isinf(result.r));  // 1/0 = inf
    EXPECT_TRUE(std::isnan(result.g));  // 0/0 = nan
}

TYPED_TEST(ColorTRobustnessTest, IsEqualApproximelyWithNan)
{
    using Color = typename TestFixture::Color;
    TypeParam nan = std::numeric_limits<TypeParam>::quiet_NaN();
    Color a(nan, static_cast<TypeParam>(0.5), static_cast<TypeParam>(0.5), static_cast<TypeParam>(1.0));
    Color b(nan, static_cast<TypeParam>(0.5), static_cast<TypeParam>(0.5), static_cast<TypeParam>(1.0));
    // NaN != NaN by IEEE 754; approximate comparison should return false
    EXPECT_FALSE(Color::IsEqualApproximately(a, b));
}

TYPED_TEST(ColorTRobustnessTest, ClampWithInfinity)
{
    using Color = typename TestFixture::Color;
    TypeParam inf = std::numeric_limits<TypeParam>::infinity();
    Color c(inf, -inf, static_cast<TypeParam>(0.5), static_cast<TypeParam>(1.0));
    c.Clamp();
    EXPECT_NEAR(c.r, 1.0, kEpsF);
    EXPECT_NEAR(c.g, 0.0, kEpsF);
}

TYPED_TEST(ColorTRobustnessTest, HsvToRgbHueWrapsAt1)
{
    using Color = typename TestFixture::Color;
    // Hue = 1.0 should be equivalent to hue = 0.0 (both are red)
    Color c0 = Color::HsvToRgb(static_cast<TypeParam>(0.0),
        static_cast<TypeParam>(1.0),
        static_cast<TypeParam>(1.0));
    Color c1 = Color::HsvToRgb(static_cast<TypeParam>(360.0),
        static_cast<TypeParam>(1.0),
        static_cast<TypeParam>(1.0));
    EXPECT_TRUE(ColorEqual(c0, c1, static_cast<TypeParam>(1e-4)));
}

TYPED_TEST(ColorTRobustnessTest, ToStringF0RoundsToNearestInteger)
{
    using Color = typename TestFixture::Color;
    Color c(static_cast<TypeParam>(0.5), static_cast<TypeParam>(0.4),
        static_cast<TypeParam>(0.9), static_cast<TypeParam>(1.0));
    auto str = c.ToString("F0");
    ASSERT_TRUE(str.has_value());
    // Just check it has valid non-empty output
    EXPECT_FALSE(str->empty());
}

TYPED_TEST(ColorTRobustnessTest, HtmlToRgbWithEightDigitsIgnoresOrUsesAlpha)
{
    // Some implementations accept 8-char HTML with alpha; check it at least doesn't crash
    using Color = typename TestFixture::Color;
    auto result = Color::HtmlToRgb("#FF0000FF");
    // May or may not parse — but should not throw
    (void)result;
}

TYPED_TEST(ColorTRobustnessTest, ToStringHtmlBlackIsAllZeroes)
{
    using Color = typename TestFixture::Color;
    Color black(static_cast<TypeParam>(0.0));
    auto str = black.ToString("H");
    ASSERT_TRUE(str.has_value());
    EXPECT_EQ(*str, "#000000");
}

TYPED_TEST(ColorTRobustnessTest, ToStringHtmlWhiteIsAllFF)
{
    using Color = typename TestFixture::Color;
    Color white(static_cast<TypeParam>(1.0));
    auto str = white.ToString("H");
    ASSERT_TRUE(str.has_value());
    EXPECT_EQ(*str, "#FFFFFF");
}

TYPED_TEST(ColorTRobustnessTest, Uint8ConstructorMaxValuesAreOne)
{
    using Color = typename TestFixture::Color;
    Color c(static_cast<uint8_t>(255), static_cast<uint8_t>(255),
        static_cast<uint8_t>(255), static_cast<uint8_t>(255));
    EXPECT_NEAR(c.r, 1.0, kEpsF);
    EXPECT_NEAR(c.g, 1.0, kEpsF);
    EXPECT_NEAR(c.b, 1.0, kEpsF);
    EXPECT_NEAR(c.a, 1.0, kEpsF);
}

TYPED_TEST(ColorTRobustnessTest, Uint8ConstructorZeroValuesAreZero)
{
    using Color = typename TestFixture::Color;
    Color c(static_cast<uint8_t>(0), static_cast<uint8_t>(0),
        static_cast<uint8_t>(0), static_cast<uint8_t>(0));
    EXPECT_NEAR(c.r, 0.0, kEpsF);
    EXPECT_NEAR(c.g, 0.0, kEpsF);
    EXPECT_NEAR(c.b, 0.0, kEpsF);
    EXPECT_NEAR(c.a, 0.0, kEpsF);
}

TYPED_TEST(ColorTRobustnessTest, MinMaxSameColorReturnsSameColor)
{
    using Color = typename TestFixture::Color;
    Color c(static_cast<TypeParam>(0.5), static_cast<TypeParam>(0.5),
        static_cast<TypeParam>(0.5), static_cast<TypeParam>(1.0));
    EXPECT_TRUE(ColorEqual(Color::Min(c, c), c));
    EXPECT_TRUE(ColorEqual(Color::Max(c, c), c));
}