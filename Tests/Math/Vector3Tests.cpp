#include <gtest/gtest.h>
#include <cmath>

#include "ByteEngine/Math/Rotation.h"
#include "ByteEngine/Math/Vector3.h"
#include "ByteEngine/Math/Vector2.h"
#include "ByteEngine/Math/Vector4.h"
#include "ByteEngine/Math/VectorConversions.h"

using namespace ByteEngine::Math;

template <typename T>
class Vector3tTest : public ::testing::Test
{
protected:
    using Vec3 = Vector3T<T>;
};

template <typename T>
class Vector3tFloatTypesTest : public ::testing::Test
{
protected:
    using Vec3 = Vector3T<T>;
};

using Types = ::testing::Types<float, double, ByteEngine::int32, ByteEngine::int64>;
using FloatTypes = ::testing::Types<float, double>;

TYPED_TEST_SUITE(Vector3tTest, Types);
TYPED_TEST_SUITE(Vector3tFloatTypesTest, FloatTypes);

TYPED_TEST(Vector3tTest, LengthAndNormalize)
{
    using Vec3 = typename TestFixture::Vec3;
    Vec3 v(3.0, 4.0, 5.0);
    EXPECT_NEAR(v.Length(), 7.071, 6.782e-05);
    EXPECT_NEAR(v.LengthSquared(), 50, 1e-5);

    if constexpr (std::floating_point<decltype(v.x)>)
    {
        Vec3 normalized = v.Normalized();
        EXPECT_NEAR(normalized.Length(), 1.0, 1e-5);
        EXPECT_TRUE(normalized.IsNormalized());

        v.Normalize();
        EXPECT_TRUE(v.IsNormalized());
        EXPECT_NEAR(v.Length(), 1.0, 1e-5);

        Vec3 v2(1.0, 1.0, 1.0);
        v2.LimitLength(0.5);
        EXPECT_NEAR(v2.Length(), 0.5, 1e-5);
    }
}

TYPED_TEST(Vector3tFloatTypesTest, Rotation)
{
    using Vec3 = typename TestFixture::Vec3;
    using RadianT = RadianT<typename Vec3::FloatT>;
    Vec3 v(1.0, 0.0, 0.0);
    Vec3 axis(0.0, 1.0, 0.0);

    v.RotateBy(RadianT(Mathf::PI_D / 2.0f), axis);

    EXPECT_NEAR(v.x, 0.0, 1e-5);
    EXPECT_NEAR(v.y, 0.0, 1e-5);
    EXPECT_NEAR(v.z, 1.0, 1e-5);

    if constexpr (std::same_as<typename Vec3::FloatT, float>)
    {
        Vec3 v2(1.0, 0.0, 0.0);
        Rotation rot(90.0_df, 90.0_df, 0.0_df);
        Vec3 rotated = rot.RotateVector(v2);
        EXPECT_NEAR(rotated.x, 0.0, 1e-5);
        EXPECT_NEAR(rotated.y, 0.0, 1e-5);
        EXPECT_NEAR(rotated.z, -1.0, 1e-5);

        Vec3 v3(0.0, 2.0, 1.0);
        Quaternion q = Quaternion::FromEuler(0.0_df, 90.0_df, 180.0_df);
        rotated = q * v3;
        EXPECT_NEAR(rotated.x, 1.0, 1e-5);
        EXPECT_NEAR(rotated.y, -2.0, 1e-5);
        EXPECT_NEAR(rotated.z, 0.0, 1e-5);

        Quaternion q1 = Quaternion::FromEuler(45.0_df, 90.0_df, 0.0_df);
        Rotation rot1(45.0_df, 90.0_df, 0.0_df);
        Vec3 vec4(1.0, 0.0, 0.0);
        Vec3 rotated1 = q1 * vec4;
        Vec3 rotated2 = rot1.RotateVector(vec4);
        EXPECT_TRUE(Vec3::IsEqualApproximetly(rotated1, rotated2));
    }
}

TYPED_TEST(Vector3tFloatTypesTest, AngleMethods)
{
    using Vec3 = typename TestFixture::Vec3;
    Vec3 a(1.0, 0.0, 0.0);
    Vec3 b(0.0, 1.0, 0.0);
    Vec3 axis(0.0, 0.0, 1.0);

    EXPECT_NEAR(Vec3::AngleBetween(a, b, axis).value, Mathf::PI / 2.0f, 1e-4);
    EXPECT_NEAR(Vec3::UnsigedAngleBetween(a, b).value, Mathf::PI / 2.0f, 1e-4);
}

TYPED_TEST(Vector3tTest, AlgebraMethods)
{
    using Vec3 = typename TestFixture::Vec3;
    Vec3 a(2.0, 3.0, 1.0);
    Vec3 b(1.0, 5.0, 2.0);

    EXPECT_NEAR(Vec3::Distcance(a, b), std::sqrt(6.0), 1e-5);
    EXPECT_NEAR(Vec3::DistcanceSquared(a, b), 6.0, 1e-5);

    if constexpr (std::floating_point<decltype(a.x)>)
    {
        Vec3 cross = Vec3::Cross(a, b);
        EXPECT_NEAR(cross.x, 1.0, 1e-5);
        EXPECT_NEAR(cross.y, -3.0, 1e-5);
        EXPECT_NEAR(cross.z, 7.0, 1e-5);

        EXPECT_NEAR(Vec3::Dot(a, b), 19.0, 1e-5);

        Vec3 p = Vec3::Project(Vec3(2, 2, 2), Vec3(1, 0, 0));
        EXPECT_NEAR(p.x, 2.0, 1e-5);
        EXPECT_NEAR(p.y, 0.0, 1e-5);
        EXPECT_NEAR(p.z, 0.0, 1e-5);
    }

    Vec3 dir = Vec3::Direction(Vec3(0, 0, 0), Vec3(1, 1, 1));

    if constexpr (std::floating_point<decltype(dir.x)>)
    {
        EXPECT_NEAR(dir.x, 0.57735, 1e-4);
        EXPECT_NEAR(dir.y, 0.57735, 1e-4);
        EXPECT_NEAR(dir.z, 0.57735, 1e-4);
    }
    else
    {
        EXPECT_EQ(dir.x, 1);
        EXPECT_EQ(dir.y, 1);
        EXPECT_EQ(dir.z, 1);
    }
}

TYPED_TEST(Vector3tFloatTypesTest, InterpolationAndReflection)
{
    using Vec3 = typename TestFixture::Vec3;
    Vec3 start(0, 0, 0);
    Vec3 end(10, 10, 10);

    EXPECT_EQ(Vec3::Lerp(start, end, 0.5), Vec3(5, 5, 5));
    EXPECT_EQ(Vec3::LerpClamped(start, end, 2.0), Vec3(10, 10, 10));

    Vec3 current(0, 0, 0);
    Vec3 target(10, 0, 0);
    Vec3 moved = Vec3::MoveTowards(current, target, 5.0);
    EXPECT_EQ(moved, Vec3(5, 0, 0));

    Vec3 vec(1, -1, 0);
    Vec3 normal(0, 1, 0);
    Vec3 reflection = Vec3::Reflect(vec, normal);
    EXPECT_NEAR(reflection.x, 1.0, 1e-5);
    EXPECT_NEAR(reflection.y, 1.0, 1e-5);
    EXPECT_NEAR(reflection.z, 0.0, 1e-5);
}

TYPED_TEST(Vector3tTest, MinMax)
{
    using Vec3 = typename TestFixture::Vec3;

    Vec3 a(1, 10, 4);
    Vec3 b(7, 4, 5);
    Vec3 c(20, 20, -1);

    Vec3 min = Vec3::Min(a, b);
    EXPECT_EQ(min.x, 1);
    EXPECT_EQ(min.y, 4);
    EXPECT_EQ(min.z, 4);

    min = Vec3::Min(a, b, c);
    EXPECT_EQ(min.x, 1);
    EXPECT_EQ(min.y, 4);
    EXPECT_EQ(min.z, -1);

    Vec3 max = Vec3::Max(a, b);
    EXPECT_EQ(max.x, 7);
    EXPECT_EQ(max.y, 10);
    EXPECT_EQ(max.z, 5);

    max = Vec3::Max(a, b, c);
    EXPECT_EQ(max.x, 20);
    EXPECT_EQ(max.y, 20);
    EXPECT_EQ(max.z, 5);
}

TYPED_TEST(Vector3tTest, Operators)
{
    using Vec3 = typename TestFixture::Vec3;
    Vec3 v1(2.0, 3.0, 4.0);
    Vec3 v2(1.0, 2.0, 3.0);

    EXPECT_EQ(+v1, Vec3(2, 3, 4));
    EXPECT_EQ(-v1, Vec3(-2, -3, -4));
    EXPECT_EQ(v1 + v2, Vec3(3, 5, 7));
    EXPECT_EQ(v1 + 2.0, Vec3(4, 5, 6));
    EXPECT_EQ(v1 - v2, Vec3(1, 1, 1));
    EXPECT_EQ(v1 - 1.0, Vec3(1, 2, 3));
    EXPECT_EQ(v1 * v2, Vec3(2, 6, 12));
    EXPECT_EQ(v1 * 2.0, Vec3(4, 6, 8));
    EXPECT_EQ(v1 / v2, Vec3(2, 1.5, 4.0 / 3.0));

    Vec3 v3 = v1;
    v3 += v2;
    EXPECT_EQ(v3, Vec3(3, 5, 7));
    v3 -= v2;
    EXPECT_EQ(v3, v1);

    EXPECT_TRUE(v1 == Vec3(2, 3, 4));
    EXPECT_TRUE(v1 != v2);
}

TYPED_TEST(Vector3tTest, AccessAndConversion)
{
    using Vec3 = typename TestFixture::Vec3;
    Vec3 v(1.0, 2.0, 3.0);

    EXPECT_EQ(v[0], 1.0);
    EXPECT_EQ(v[1], 2.0);
    EXPECT_EQ(v[2], 3.0);

    Vector2T<decltype(v.x)> v2 = v;
    EXPECT_EQ(v2.x, 1.0);
    EXPECT_EQ(v2.y, 2.0);

    Vector4D v4 = v;
    EXPECT_EQ(v4.x, 1.0);
    EXPECT_EQ(v4.y, 2.0);
    EXPECT_EQ(v4.z, 3.0);
    EXPECT_EQ(v4.w, 0.0);
}

TYPED_TEST(Vector3tFloatTypesTest, IsEqualApproximetly)
{
    using Vec3 = typename TestFixture::Vec3;
    Vec3 v1(1.0000000001, 1.0, 1.0);
    Vec3 v2(1.0, 1.0, 1.0);
    EXPECT_TRUE(Vec3::IsEqualApproximetly(v1, v2));
}

template <typename T>
class Vector3tFloatTypesRobustnessTest : public ::testing::Test
{
protected:
    using Vec3 = Vector3T<T>;
};

template <typename T>
class Vector3tIntTypesRobustnessTest : public ::testing::Test
{
protected:
    using Vec3 = Vector3T<T>;
};

using FloatTypes = ::testing::Types<float, double>;
using IntTypes = ::testing::Types<ByteEngine::int32, ByteEngine::int64>;

TYPED_TEST_SUITE(Vector3tFloatTypesRobustnessTest, FloatTypes);
TYPED_TEST_SUITE(Vector3tIntTypesRobustnessTest, IntTypes);

TYPED_TEST(Vector3tFloatTypesRobustnessTest, VectorNormalization)
{
    using Vec3 = typename TestFixture::Vec3;
    Vec3 zero(0);

    zero.Normalize();
    EXPECT_TRUE(zero.x == 0 && zero.y == 0 && zero.z == 0);

    Vec3 inf(Mathf::Infinity, Mathf::Infinity, Mathf::Infinity);
    inf.Normalize();
    EXPECT_TRUE(std::isnan(inf.x) && std::isnan(inf.y) && std::isnan(inf.z));

    Vec3 mixed(Mathf::Infinity, 1213, 0);
    mixed.Normalize();
    EXPECT_TRUE(std::isnan(mixed.x) && mixed.y == 0 && mixed.z == 0);

    Vec3 nan(std::numeric_limits<decltype(zero.x)>::quiet_NaN());
    nan.Normalize();
    EXPECT_TRUE(nan.x == 0 && nan.y == 0 && nan.z == 0);
}

TYPED_TEST(Vector3tFloatTypesRobustnessTest, RotationWithZeroAxis)
{
    using Vec3 = typename TestFixture::Vec3;
    using RadianT = RadianT<typename Vec3::FloatT>;
    Vec3 v(1.0, 0.0, 0.0);
    Vec3 zeroAxis(0.0, 0.0, 0.0);

    v.RotateBy(RadianT(Mathf::PI_D / 2.0f), zeroAxis);

    EXPECT_NEAR(v.x, 1.0, 1e-5);
    EXPECT_NEAR(v.y, 0.0, 1e-5);
    EXPECT_NEAR(v.z, 0.0, 1e-5);
}

TYPED_TEST(Vector3tFloatTypesRobustnessTest, RotateByPrecision)
{
    using Vec3 = typename TestFixture::Vec3;
    using RadianT = RadianT<typename Vec3::FloatT>;
    Vec3 v(1.0, 0.0, 0.0);
    Vec3 axis(0.0, 1.0, 0.0);

    Vec3 rotated1 = v.RotatedBy(RadianT(Mathf::PI_D * 2.0f * 10000), axis);

    EXPECT_NEAR(rotated1.x, 1.0f, 1.4e-2f);
    EXPECT_NEAR(rotated1.y, 0.0f, 1.4e-2f);
    EXPECT_NEAR(rotated1.z, 0.0f, 1.4e-2f);

    Vec3 rotated2 = v.RotatedBy(RadianT(0.0000001), axis);
    EXPECT_NEAR(rotated2.x, 1.0f, 1e-5f);
    EXPECT_NEAR(rotated2.y, 0.0f, 1e-5f);
    EXPECT_NEAR(rotated2.z, 0.0f, 1e-5f);
}

TYPED_TEST(Vector3tFloatTypesRobustnessTest, DivisionByZero)
{
    using Vec3 = typename TestFixture::Vec3;
    Vec3 v(1.0, 1.0, 1.0);
    Vec3 zero(0, 0, 0);

    Vec3 result = v / 0;
    EXPECT_TRUE(std::isinf(result.x) && std::isinf(result.y) && std::isinf(result.z));

    result = zero / 0;
    EXPECT_TRUE(std::isnan(result.x) && std::isnan(result.y) && std::isnan(result.z));
}

TYPED_TEST(Vector3tFloatTypesRobustnessTest, ExtremeValuesOverflow)
{
    using Vec3 = typename TestFixture::Vec3;
    auto maxVal = std::numeric_limits<decltype(Vec3(0).x)>::max();

    Vec3 v(maxVal, maxVal, maxVal);
    auto sq = v.LengthSquared();
    auto len = v.Length();

    EXPECT_TRUE(std::isinf(sq));
    EXPECT_TRUE(std::isinf(len));
}

TYPED_TEST(Vector3tIntTypesRobustnessTest, ExtremeValuesOverflow)
{
    using Vec3 = typename TestFixture::Vec3;
    auto maxVal = std::numeric_limits<decltype(Vec3(0).x)>::max();

    Vec3 v(maxVal, maxVal, maxVal);
    auto sq = v.LengthSquared();
    EXPECT_GT(sq, 0);
}

TYPED_TEST(Vector3tFloatTypesRobustnessTest, AngleBetweenZeroVectors)
{
    using Vec3 = typename TestFixture::Vec3;
    Vec3 zero(0, 0, 0);
    Vec3 normal(1, 0, 0);
    Vec3 axis(0, 1, 0);

    auto angle = Vec3::AngleBetween(zero, normal, axis);
    EXPECT_EQ(angle.value, 0);

    angle = Vec3::AngleBetween(axis, normal, zero);
    EXPECT_NEAR(angle.value, Mathf::PI / 2, 1e-4f);
}

TYPED_TEST(Vector3tFloatTypesRobustnessTest, ProjectAndReflectWithZeroNormal)
{
    using Vec3 = typename TestFixture::Vec3;
    Vec3 v(1, 1, 1);
    Vec3 zeroNormal(0, 0, 0);

    Vec3 proj = Vec3::Project(v, zeroNormal);
    EXPECT_EQ(proj.x, 0);
    EXPECT_EQ(proj.y, 0);
    EXPECT_EQ(proj.z, 0);

    Vec3 refl = Vec3::Reflect(v, zeroNormal);
    EXPECT_EQ(refl.x, v.x);
    EXPECT_EQ(refl.y, v.y);
    EXPECT_EQ(refl.z, v.z);
}

TYPED_TEST(Vector3tFloatTypesRobustnessTest, MoveTowardsBounds)
{
    using Vec3 = typename TestFixture::Vec3;
    Vec3 current(0, 0, 0);
    Vec3 target(10, 10, 10);

    Vec3 res = Vec3::MoveTowards(current, target, 1000.0f);
    EXPECT_EQ(res, target);

    res = Vec3::MoveTowards(current, target, 0.0f);
    EXPECT_EQ(res, current);
}