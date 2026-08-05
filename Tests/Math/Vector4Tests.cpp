#include <gtest/gtest.h>
#include <cmath>
#include <limits>
#include "ByteEngine/Math/Vector4.h"
#include "ByteEngine/Math/Vector3.h"
#include "ByteEngine/Math/Vector2.h"
#include "ByteEngine/Math/VectorConversions.h"

using namespace ByteEngine::Math;

template <typename T>
class Vector4tTest : public ::testing::Test { protected: using Vec4 = Vector4T<T>; };

template <typename T>
class Vector4tFloatTypesTest : public ::testing::Test { protected: using Vec4 = Vector4T<T>; };

using Types = ::testing::Types<float, double, ByteEngine::int32, ByteEngine::int64>;
using FloatTypes = ::testing::Types<float, double>;
using IntTypes = ::testing::Types<ByteEngine::int32, ByteEngine::int64>;

TYPED_TEST_SUITE(Vector4tTest, Types);
TYPED_TEST_SUITE(Vector4tFloatTypesTest, FloatTypes);

TYPED_TEST(Vector4tTest, LengthAndNormalize)
{
    using Vec4 = typename TestFixture::Vec4;
    Vec4 v(1.0, 1.0, 1.0, 1.0);
    EXPECT_NEAR(v.Length(), 2.0, 1e-5);
    EXPECT_NEAR(v.LengthSquared(), 4.0, 1e-5);

    if constexpr (std::floating_point<decltype(v.x)>)
    {
        Vec4 normalized = v.Normalized();
        EXPECT_NEAR(normalized.Length(), 1.0, 1e-5);
        EXPECT_TRUE(normalized.IsNormalized());

        v.Normalize();
        EXPECT_TRUE(v.IsNormalized());
        EXPECT_NEAR(v.Length(), 1.0, 1e-5);

        Vec4 v2(2.0, 0.0, 0.0, 0.0);
        v2.LimitLength(1.0);
        EXPECT_NEAR(v2.Length(), 1.0, 1e-5);
    }
}

TYPED_TEST(Vector4tTest, AlgebraMethods)
{
    using Vec4 = typename TestFixture::Vec4;
    Vec4 a(2.0, 3.0, 4.0, 5.0);
    Vec4 b(1.0, 2.0, 1.0, 2.0);

    EXPECT_NEAR(Vec4::Distcance(a, b), 3.31662, 1e-4);
    EXPECT_NEAR(Vec4::DistcanceSquared(a, b), 11.0, 1e-5);
    EXPECT_NEAR(Vec4::Dot(a, b), 2.0 + 6.0 + 4.0 + 10.0, 1e-5);

    Vec4 dir = Vec4::Direction(Vec4(0), Vec4(1, 1, 1, 1));
    if constexpr (std::floating_point<decltype(dir.x)>)
        EXPECT_NEAR(dir.x, 0.5, 1e-4);
}

TYPED_TEST(Vector4tFloatTypesTest, Interpolation)
{
    using Vec4 = typename TestFixture::Vec4;
    Vec4 start(0, 0, 0, 0);
    Vec4 end(10, 10, 10, 10);

    EXPECT_EQ(Vec4::Lerp(start, end, 0.5), Vec4(5, 5, 5, 5));
    EXPECT_EQ(Vec4::LerpClamped(start, end, 2.0), Vec4(10, 10, 10, 10));

    Vec4 current(0, 0, 0, 0);
    Vec4 target(5, 0, 0, 0);
    Vec4 moved = Vec4::MoveTowards(current, target, 2.0);
    EXPECT_EQ(moved, Vec4(2, 0, 0, 0));
}

TYPED_TEST(Vector4tTest, MinMax)
{
    using Vec4 = typename TestFixture::Vec4;

    Vec4 a(1, 10, 4, -7);
    Vec4 b(7, 4, 5, 100);
    Vec4 c(20, 20, -1, 0);

    Vec4 min = Vec4::Min(a, b);
    EXPECT_EQ(min.x, 1);
    EXPECT_EQ(min.y, 4);
    EXPECT_EQ(min.z, 4);
    EXPECT_EQ(min.w, -7);

    min = Vec4::Min(a, b, c);
    EXPECT_EQ(min.x, 1);
    EXPECT_EQ(min.y, 4);
    EXPECT_EQ(min.z, -1);
    EXPECT_EQ(min.w, -7);

    Vec4 max = Vec4::Max(a, b);
    EXPECT_EQ(max.x, 7);
    EXPECT_EQ(max.y, 10);
    EXPECT_EQ(max.z, 5);
    EXPECT_EQ(max.w, 100);

    max = Vec4::Max(a, b, c);
    EXPECT_EQ(max.x, 20);
    EXPECT_EQ(max.y, 20);
    EXPECT_EQ(max.z, 5);
    EXPECT_EQ(max.w, 100);
}

TYPED_TEST(Vector4tTest, Operators)
{
    using Vec4 = typename TestFixture::Vec4;
    Vec4 v1(2, 2, 2, 2);
    Vec4 v2(1, 1, 1, 1);

    EXPECT_EQ(+v1, Vec4(2, 2, 2, 2));
    EXPECT_EQ(-v1, Vec4(-2, -2, -2, -2));
    EXPECT_EQ(v1 + v2, Vec4(3, 3, 3, 3));
    EXPECT_EQ(v1 - v2, Vec4(1, 1, 1, 1));
    EXPECT_EQ(v1 * v2, Vec4(2, 2, 2, 2));
    EXPECT_EQ(v1 / 2.0, Vec4(1, 1, 1, 1));

    v1 += v2;
    EXPECT_EQ(v1, Vec4(3, 3, 3, 3));
}

TYPED_TEST(Vector4tTest, ConversionsAndAccess)
{
    using Vec4 = typename TestFixture::Vec4;
    Vec4 v(1.0, 2.0, 3.0, 4.0);

    EXPECT_EQ(v[0], 1.0);
    EXPECT_EQ(v[3], 4.0);
    v[0] = 10.0;
    EXPECT_EQ(v.x, 10.0);

    Vector3T<decltype(v.x)> v3 = v;
    EXPECT_EQ(v3.z, 3.0);

    Vector2T<double> v2 = v;
    EXPECT_EQ(v2.y, 2.0);
}

template <typename T> class Vector4tFloatTypesRobustnessTest : public ::testing::Test { protected: using Vec4 = Vector4T<T>; };
template <typename T> class Vector4tIntTypesRobustnessTest : public ::testing::Test { protected: using Vec4 = Vector4T<T>; };

TYPED_TEST_SUITE(Vector4tFloatTypesRobustnessTest, FloatTypes);
TYPED_TEST_SUITE(Vector4tIntTypesRobustnessTest, IntTypes);

TYPED_TEST(Vector4tFloatTypesRobustnessTest, NormalizationRobustness)
{
    using Vec4 = typename TestFixture::Vec4;
    Vec4 zero(0);

    zero.Normalize();
    EXPECT_TRUE(zero.x == 0 && zero.y == 0 && zero.z == 0 && zero.w == 0);

    Vec4 inf(Mathf::Infinity);
    inf.Normalize();
    EXPECT_TRUE(std::isnan(inf.x) && std::isnan(inf.y) && std::isnan(inf.z));

    Vec4 mixed(Mathf::Infinity, 1213, 0, 1);
    mixed.Normalize();
    EXPECT_TRUE(std::isnan(mixed.x) && mixed.y == 0 && mixed.z == 0);

    Vec4 nan(std::numeric_limits<decltype(zero.x)>::quiet_NaN());
    nan.Normalize();
    EXPECT_TRUE(nan.x == 0 && nan.y == 0 && nan.z == 0);
}

TYPED_TEST(Vector4tFloatTypesRobustnessTest, DivisionByZero)
{
    using Vec4 = typename TestFixture::Vec4;
    Vec4 v(1.0, 1.0, 1.0, 1.0);
    Vec4 res = v / 0.0;
    EXPECT_TRUE(std::isinf(res.x) && std::isinf(res.y) && std::isinf(res.z) && std::isinf(res.w));
}

TYPED_TEST(Vector4tFloatTypesRobustnessTest, ExtremeValues)
{
    using Vec4 = typename TestFixture::Vec4;
    auto maxVal = std::numeric_limits<decltype(Vec4(0).x)>::max();

    Vec4 v(maxVal);

    EXPECT_TRUE(std::isinf(v.Length()));
    EXPECT_TRUE(std::isinf(v.LengthSquared()));
}

TYPED_TEST(Vector4tFloatTypesRobustnessTest, IsEqualApproximetly)
{
    using Vec4 = typename TestFixture::Vec4;
    Vec4 v1(1.0000000001, 1.0, 1.0, 1.0);
    Vec4 v2(1.0, 1.0, 1.0, 1.0);
    EXPECT_TRUE(Vec4::IsEqualApproximetly(v1, v2));
}