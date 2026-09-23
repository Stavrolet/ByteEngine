#include <gtest/gtest.h>
#include "ByteEngine/Math/Quaternion.h"
#include "ByteEngine/Math/Vector3.h"
#include "ByteEngine/Math/Math.h"

using namespace ByteEngine::Math;
using namespace ByteEngine::Math::Literals;

template <typename T>
class QuaterniontTest : public ::testing::Test
{
protected:
    using Quat = QuaternionT<T>;
    using Vec3 = Vector3T<T>;
    using RadianT = RadianT<T>;
    using DegreeT = DegreeT<T>;
};

using FloatTypes = testing::Types<float, double>;
TYPED_TEST_SUITE(QuaterniontTest, FloatTypes);

TYPED_TEST(QuaterniontTest, LengthSquared)
{
    using Quat = TestFixture::Quat;
    Quat q(1.0, 2.0, 2.0, 0.0);
    EXPECT_NEAR(q.LengthSquared(), 9.0, 1e-5);
}

TYPED_TEST(QuaterniontTest, Length)
{
    using Quat = TestFixture::Quat;
    Quat q(1.0, 2.0, 2.0, 0.0);
    EXPECT_NEAR(q.Length(), 3.0, 1e-5);
}

TYPED_TEST(QuaterniontTest, ZeroQuaternionLength)
{
    using Quat = TestFixture::Quat;
    Quat q(0.0);
    EXPECT_NEAR(q.Length(), 0.0, 1e-5);
}

TYPED_TEST(QuaterniontTest, Normalize)
{
    using Quat = TestFixture::Quat;
    Quat q(1.0, 2.0, 2.0, 0.0);
    q.Normalize();
    EXPECT_NEAR(q.Length(), 1.0, 1e-5);
    EXPECT_TRUE(q.IsNormalized());
}

TYPED_TEST(QuaterniontTest, Normalized)
{
    using Quat = TestFixture::Quat;
    Quat q(1.0, 2.0, 2.0, 0.0);
    Quat normalized = q.Normalized();
    EXPECT_NEAR(normalized.Length(), 1.0, 1e-5);
    EXPECT_TRUE(normalized.IsNormalized());
    EXPECT_NEAR(q.Length(), 3.0, 1e-5);
}

TYPED_TEST(QuaterniontTest, IsNormalized)
{
    using Quat = TestFixture::Quat;
    Quat normalized(0.577, 0.577, 0.577, 0.0);
    normalized.Normalize();
    EXPECT_TRUE(normalized.IsNormalized());

    Quat notNormalized(1.0, 2.0, 3.0, 4.0);
    EXPECT_FALSE(notNormalized.IsNormalized());
}

TYPED_TEST(QuaterniontTest, IsEqualApproximately)
{
    using Quat = TestFixture::Quat;
    Quat q1(1.00001, 2.00001, 3.00001, 4.00001);
    Quat q2(1.00002, 2.00002, 3.00002, 4.00002);
    EXPECT_TRUE(Quat::IsEqualApproximately(q1, q2, static_cast<TypeParam>(0.0001)));
    EXPECT_FALSE(Quat::IsEqualApproximately(q1, q2, static_cast<TypeParam>(0.000001)));
}

TYPED_TEST(QuaterniontTest, Inverse)
{
    using Quat = TestFixture::Quat;
    Quat q = Quat(0.0, 0.0, 1.0, 0.0).Normalized();
    q.Inverse();
    EXPECT_NEAR(q.x, 0.0, 1e-5);
    EXPECT_NEAR(q.y, 0.0, 1e-5);
    EXPECT_NEAR(q.z, -1.0, 1e-5);
    EXPECT_NEAR(q.w, 0.0, 1e-5);
}

TYPED_TEST(QuaterniontTest, Inversed)
{
    using Quat = TestFixture::Quat;
    Quat q = Quat(0.0, 0.0, 1.0, 0.0).Normalized();
    Quat inversed = q.Inversed();
    EXPECT_NEAR(inversed.x, 0.0, 1e-5);
    EXPECT_NEAR(inversed.y, 0.0, 1e-5);
    EXPECT_NEAR(inversed.z, -1.0, 1e-5);
    EXPECT_NEAR(inversed.w, 0.0, 1e-5);
    EXPECT_NEAR(q.z, 1.0, 1e-5);
}

TYPED_TEST(QuaterniontTest, FromEulerInRadians)
{
    using Quat = TestFixture::Quat;
    using RadianT = TestFixture::RadianT;
    Quat q = Quat::FromEuler(RadianT(TypeParam(0)), RadianT(TypeParam(0)), RadianT(TypeParam(0)));
    EXPECT_NEAR(q.x, 0.0, 1e-5);
    EXPECT_NEAR(q.y, 0.0, 1e-5);
    EXPECT_NEAR(q.z, 0.0, 1e-5);
    EXPECT_NEAR(q.w, 1.0, 1e-5);
}

TYPED_TEST(QuaterniontTest, FromEulerInDegrees)
{
    using Quat = TestFixture::Quat;
    using DegreeT = TestFixture::DegreeT;
    Quat q = Quat::FromEuler(DegreeT(TypeParam(0)), DegreeT(TypeParam(0)), DegreeT(TypeParam(0)));
    EXPECT_NEAR(q.x, 0.0, 1e-5);
    EXPECT_NEAR(q.y, 0.0, 1e-5);
    EXPECT_NEAR(q.z, 0.0, 1e-5);
    EXPECT_NEAR(q.w, 1.0, 1e-5);
}

TYPED_TEST(QuaterniontTest, FromEulerRadianComponents)
{
    using Quat = TestFixture::Quat;
    using RadianT = TestFixture::RadianT;
    Quat q = Quat::FromEuler(RadianT(TypeParam(0)), RadianT(TypeParam(0)), RadianT(TypeParam(0)));
    EXPECT_NEAR(q.x, 0.0, 1e-5);
    EXPECT_NEAR(q.y, 0.0, 1e-5);
    EXPECT_NEAR(q.z, 0.0, 1e-5);
    EXPECT_NEAR(q.w, 1.0, 1e-5);
}

TYPED_TEST(QuaterniontTest, FromEulerDegreeComponents)
{
    using Quat = TestFixture::Quat;
    using DegreeT = TestFixture::DegreeT;
    Quat q = Quat::FromEuler(DegreeT(TypeParam(0)), DegreeT(TypeParam(0)), DegreeT(TypeParam(0)));
    EXPECT_NEAR(q.x, 0.0, 1e-5);
    EXPECT_NEAR(q.y, 0.0, 1e-5);
    EXPECT_NEAR(q.z, 0.0, 1e-5);
    EXPECT_NEAR(q.w, 1.0, 1e-5);
}

TYPED_TEST(QuaterniontTest, GetEulerInRadians)
{
    using Quat = TestFixture::Quat;
    using DegreeT = TestFixture::DegreeT;
    Quat q = Quat::FromEuler(DegreeT(TypeParam(0)), DegreeT(TypeParam(0)), DegreeT(TypeParam(0)));
    auto euler = q.GetEuler();
    EXPECT_NEAR(euler.pitch.value, 0.0, 1e-5);
    EXPECT_NEAR(euler.yaw.value, 0.0, 1e-5);
    EXPECT_NEAR(euler.roll.value, 0.0, 1e-5);
}

TYPED_TEST(QuaterniontTest, GetEuler)
{
    using Quat = TestFixture::Quat;
    using DegreeT = TestFixture::DegreeT;
    Quat q = Quat::FromEuler(DegreeT(TypeParam(0)), DegreeT(TypeParam(0)), DegreeT(TypeParam(0)));
    auto euler = q.GetEuler();
    EXPECT_NEAR(euler.pitch.value, 0.0, 1e-5);
    EXPECT_NEAR(euler.yaw.value, 0.0, 1e-5);
    EXPECT_NEAR(euler.roll.value, 0.0, 1e-5);
}

TYPED_TEST(QuaterniontTest, FromAngleAxisRadian)
{
    using Quat = TestFixture::Quat;
    using Vec3 = TestFixture::Vec3;
    using RadianT = TestFixture::RadianT;
    Vec3 axis(0.0, 1.0, 0.0);
    RadianT angle(TypeParam(0));
    Quat q = Quat::FromAngleAxis(angle, axis);
    EXPECT_NEAR(q.x, 0.0, 1e-5);
    EXPECT_NEAR(q.y, 0.0, 1e-5);
    EXPECT_NEAR(q.z, 0.0, 1e-5);
    EXPECT_NEAR(q.w, 1.0, 1e-5);
}

TYPED_TEST(QuaterniontTest, GetAxis)
{
    using Quat = TestFixture::Quat;
    using Vec3 = TestFixture::Vec3;
    using RadianT = TestFixture::RadianT;
    Quat q = Quat::FromAngleAxis(RadianT(static_cast<TypeParam>(Mathf::PI) / TypeParam(2)), Vec3(0.0, 1.0, 0.0));
    Vec3 axis = q.GetAxis();
    EXPECT_NEAR(axis.x, 0.0, 1e-5);
    EXPECT_NEAR(std::abs(axis.y), 1.0, 1e-5);
    EXPECT_NEAR(axis.z, 0.0, 1e-5);
}

TYPED_TEST(QuaterniontTest, GetAngle)
{
    using Quat = TestFixture::Quat;
    using Vec3 = TestFixture::Vec3;
    using RadianT = TestFixture::RadianT;
    Quat q = Quat::FromAngleAxis(RadianT(static_cast<TypeParam>(Mathf::PI) / TypeParam(2)), Vec3(0.0, 1.0, 0.0));
    RadianT angle = q.GetAngle();
    EXPECT_NEAR(angle.value, static_cast<double>(Mathf::PI) / 2.0, 1e-5);
}

TYPED_TEST(QuaterniontTest, Dot)
{
    using Quat = TestFixture::Quat;
    Quat q1(1.0, 0.0, 0.0, 0.0);
    Quat q2(1.0, 0.0, 0.0, 0.0);
    EXPECT_NEAR(Quat::Dot(q1, q2), 1.0, 1e-5);

    Quat q3(1.0, 1.0, 1.0, 1.0);
    Quat q4(1.0, 1.0, 1.0, 1.0);
    EXPECT_NEAR(Quat::Dot(q3, q4), 4.0, 1e-5);
}

TYPED_TEST(QuaterniontTest, AngleBetween)
{
    using Quat = TestFixture::Quat;
    using Vec3 = TestFixture::Vec3;
    using RadianT = TestFixture::RadianT;
    Quat q1 = Quat::FromAngleAxis(RadianT(TypeParam(0)), Vec3(0.0, 1.0, 0.0));
    Quat q2 = Quat::FromAngleAxis(RadianT(TypeParam(0)), Vec3(0.0, 1.0, 0.0));
    RadianT angle = Quat::AngleBetween(q1, q2);
    EXPECT_NEAR(angle.value, 0.0, 1e-5);
}

TYPED_TEST(QuaterniontTest, UnaryPlus)
{
    using Quat = TestFixture::Quat;
    Quat q(1.0, -2.0, 3.0, -4.0);
    Quat result = +q;
    EXPECT_EQ(result.x, 1.0);
    EXPECT_EQ(result.y, -2.0);
    EXPECT_EQ(result.z, 3.0);
    EXPECT_EQ(result.w, -4.0);
}

TYPED_TEST(QuaterniontTest, UnaryMinus)
{
    using Quat = TestFixture::Quat;
    Quat q(1.0, -2.0, 3.0, -4.0);
    Quat result = -q;
    EXPECT_EQ(result.x, -1.0);
    EXPECT_EQ(result.y, 2.0);
    EXPECT_EQ(result.z, -3.0);
    EXPECT_EQ(result.w, 4.0);
}

TYPED_TEST(QuaterniontTest, Addition)
{
    using Quat = TestFixture::Quat;
    Quat q1(1.0, 2.0, 3.0, 4.0);
    Quat q2(5.0, 6.0, 7.0, 8.0);
    Quat result = q1 + q2;
    EXPECT_EQ(result.x, 6.0);
    EXPECT_EQ(result.y, 8.0);
    EXPECT_EQ(result.z, 10.0);
    EXPECT_EQ(result.w, 12.0);
}

TYPED_TEST(QuaterniontTest, Subtraction)
{
    using Quat = TestFixture::Quat;
    Quat q1(5.0, 6.0, 7.0, 8.0);
    Quat q2(1.0, 2.0, 3.0, 4.0);
    Quat result = q1 - q2;
    EXPECT_EQ(result.x, 4.0);
    EXPECT_EQ(result.y, 4.0);
    EXPECT_EQ(result.z, 4.0);
    EXPECT_EQ(result.w, 4.0);
}

TYPED_TEST(QuaterniontTest, AdditionAssignment)
{
    using Quat = TestFixture::Quat;
    Quat q1(1.0, 2.0, 3.0, 4.0);
    Quat q2(5.0, 6.0, 7.0, 8.0);
    q1 += q2;
    EXPECT_EQ(q1.x, 6.0);
    EXPECT_EQ(q1.y, 8.0);
    EXPECT_EQ(q1.z, 10.0);
    EXPECT_EQ(q1.w, 12.0);
}

TYPED_TEST(QuaterniontTest, SubtractionAssignment)
{
    using Quat = TestFixture::Quat;
    Quat q1(5.0, 6.0, 7.0, 8.0);
    Quat q2(1.0, 2.0, 3.0, 4.0);
    q1 -= q2;
    EXPECT_EQ(q1.x, 4.0);
    EXPECT_EQ(q1.y, 4.0);
    EXPECT_EQ(q1.z, 4.0);
    EXPECT_EQ(q1.w, 4.0);
}

TYPED_TEST(QuaterniontTest, Multiplication)
{
    using Quat = TestFixture::Quat;
    Quat q1(1.0, 0.0, 0.0, 0.0);
    Quat q2(0.0, 1.0, 0.0, 0.0);
    Quat result = q1 * q2;
    EXPECT_NEAR(result.x, 0.0, 1e-5);
    EXPECT_NEAR(result.y, 0.0, 1e-5);
    EXPECT_NEAR(result.z, 1.0, 1e-5);
    EXPECT_NEAR(result.w, 0.0, 1e-5);
}

TYPED_TEST(QuaterniontTest, MultiplicationAssignment)
{
    using Quat = TestFixture::Quat;
    Quat q1(1.0, 0.0, 0.0, 0.0);
    Quat q2(0.0, 1.0, 0.0, 0.0);
    q1 *= q2;
    EXPECT_NEAR(q1.x, 0.0, 1e-5);
    EXPECT_NEAR(q1.y, 0.0, 1e-5);
    EXPECT_NEAR(q1.z, 1.0, 1e-5);
    EXPECT_NEAR(q1.w, 0.0, 1e-5);
}

TYPED_TEST(QuaterniontTest, EqualityOperator)
{
    using Quat = TestFixture::Quat;
    Quat q1(1.0, 2.0, 3.0, 4.0);
    Quat q2(1.0, 2.0, 3.0, 4.0);
    Quat q3(1.0, 2.0, 3.0, 5.0);

    EXPECT_TRUE(q1 == q2);
    EXPECT_FALSE(q1 == q3);
}

TYPED_TEST(QuaterniontTest, InequalityOperator)
{
    using Quat = TestFixture::Quat;
    Quat q1(1.0, 2.0, 3.0, 4.0);
    Quat q2(1.0, 2.0, 3.0, 4.0);
    Quat q3(1.0, 2.0, 3.0, 5.0);

    EXPECT_FALSE(q1 != q2);
    EXPECT_TRUE(q1 != q3);
}

TYPED_TEST(QuaterniontTest, SubscriptOperatorRead)
{
    using Quat = TestFixture::Quat;
    Quat q(1.0, 2.0, 3.0, 4.0);
    EXPECT_EQ(q[0], 1.0);
    EXPECT_EQ(q[1], 2.0);
    EXPECT_EQ(q[2], 3.0);
    EXPECT_EQ(q[3], 4.0);
}

TYPED_TEST(QuaterniontTest, SubscriptOperatorWrite)
{
    using Quat = TestFixture::Quat;
    Quat q(0.0);
    q[0] = TypeParam(1.0);
    q[1] = TypeParam(2.0);
    q[2] = TypeParam(3.0);
    q[3] = TypeParam(4.0);
    EXPECT_EQ(q.x, 1.0);
    EXPECT_EQ(q.y, 2.0);
    EXPECT_EQ(q.z, 3.0);
    EXPECT_EQ(q.w, 4.0);
}

TYPED_TEST(QuaterniontTest, SlerpAtZero)
{
    using Quat = TestFixture::Quat;
    using Vec3 = TestFixture::Vec3;
    using RadianT = TestFixture::RadianT;
    Quat q1 = Quat::FromAngleAxis(RadianT(TypeParam(0)), Vec3(0.0, 1.0, 0.0));
    Quat q2 = Quat::FromAngleAxis(RadianT(static_cast<TypeParam>(Mathf::PI) / TypeParam(2)), Vec3(0.0, 1.0, 0.0));
    Quat result = Quat::Slerp(q1, q2, TypeParam(0));
    EXPECT_NEAR(result.x, q1.x, 1e-5);
    EXPECT_NEAR(result.y, q1.y, 1e-5);
    EXPECT_NEAR(result.z, q1.z, 1e-5);
    EXPECT_NEAR(result.w, q1.w, 1e-5);
}

TYPED_TEST(QuaterniontTest, SlerpAtOne)
{
    using Quat = TestFixture::Quat;
    using Vec3 = TestFixture::Vec3;
    using RadianT = TestFixture::RadianT;
    Quat q1 = Quat::FromAngleAxis(RadianT(TypeParam(0)), Vec3(0.0, 1.0, 0.0));
    Quat q2 = Quat::FromAngleAxis(RadianT(static_cast<TypeParam>(Mathf::PI) / TypeParam(2)), Vec3(0.0, 1.0, 0.0));
    Quat result = Quat::Slerp(q1, q2, TypeParam(1));
    EXPECT_NEAR(result.x, q2.x, 1e-5);
    EXPECT_NEAR(result.y, q2.y, 1e-5);
    EXPECT_NEAR(result.z, q2.z, 1e-5);
    EXPECT_NEAR(result.w, q2.w, 1e-5);
}

TYPED_TEST(QuaterniontTest, SlerpClampedAtZero)
{
    using Quat = TestFixture::Quat;
    using Vec3 = TestFixture::Vec3;
    using RadianT = TestFixture::RadianT;
    Quat q1 = Quat::FromAngleAxis(RadianT(TypeParam(0)), Vec3(0.0, 1.0, 0.0));
    Quat q2 = Quat::FromAngleAxis(RadianT(static_cast<TypeParam>(Mathf::PI) / TypeParam(2)), Vec3(0.0, 1.0, 0.0));
    Quat result = Quat::SlerpClamped(q1, q2, TypeParam(0));
    EXPECT_NEAR(result.x, q1.x, 1e-5);
    EXPECT_NEAR(result.y, q1.y, 1e-5);
    EXPECT_NEAR(result.z, q1.z, 1e-5);
    EXPECT_NEAR(result.w, q1.w, 1e-5);
}

TYPED_TEST(QuaterniontTest, SlerpClampedAtOne)
{
    using Quat = TestFixture::Quat;
    using Vec3 = TestFixture::Vec3;
    using RadianT = TestFixture::RadianT;
    Quat q1 = Quat::FromAngleAxis(RadianT(TypeParam(0)), Vec3(0.0, 1.0, 0.0));
    Quat q2 = Quat::FromAngleAxis(RadianT(static_cast<TypeParam>(Mathf::PI) / TypeParam(2)), Vec3(0.0, 1.0, 0.0));
    Quat result = Quat::SlerpClamped(q1, q2, TypeParam(1));
    EXPECT_NEAR(result.x, q2.x, 1e-5);
    EXPECT_NEAR(result.y, q2.y, 1e-5);
    EXPECT_NEAR(result.z, q2.z, 1e-5);
    EXPECT_NEAR(result.w, q2.w, 1e-5);
}

TYPED_TEST(QuaterniontTest, SlerpClampedClamps)
{
    using Quat = TestFixture::Quat;
    using Vec3 = TestFixture::Vec3;
    using RadianT = TestFixture::RadianT;
    Quat q1 = Quat::FromAngleAxis(RadianT(TypeParam(0)), Vec3(0.0, 1.0, 0.0));
    Quat q2 = Quat::FromAngleAxis(RadianT(static_cast<TypeParam>(Mathf::PI) / TypeParam(2)), Vec3(0.0, 1.0, 0.0));
    Quat result = Quat::SlerpClamped(q1, q2, TypeParam(1.5));
    EXPECT_NEAR(result.x, q2.x, 1e-5);
    EXPECT_NEAR(result.y, q2.y, 1e-5);
    EXPECT_NEAR(result.z, q2.z, 1e-5);
    EXPECT_NEAR(result.w, q2.w, 1e-5);
}

TYPED_TEST(QuaterniontTest, FromLookDirectionAlongZ)
{
    using Quat = TestFixture::Quat;
    using Vec3 = TestFixture::Vec3;
    Vec3 direction(0.0, 0.0, 1.0);
    Vec3 worldUp(0.0, 1.0, 0.0);
    Quat q = Quat::FromLookDirection(direction, worldUp);
    EXPECT_TRUE(q.IsNormalized());
}

TYPED_TEST(QuaterniontTest, FromLookDirectionZeroDirection)
{
    using Quat = TestFixture::Quat;
    using Vec3 = TestFixture::Vec3;
    Vec3 direction(0.0, 0.0, 0.0);
    Vec3 worldUp(0.0, 1.0, 0.0);
    Quat q = Quat::FromLookDirection(direction, worldUp);
    EXPECT_EQ(q.x, 0.0);
    EXPECT_EQ(q.y, 0.0);
    EXPECT_EQ(q.z, 0.0);
    EXPECT_EQ(q.w, 1.0);
}

TYPED_TEST(QuaterniontTest, FromToRotation)
{
    using Quat = TestFixture::Quat;
    using Vec3 = TestFixture::Vec3;
    Vec3 from(1.0, 2.0, 0.5);
    Vec3 to(0.0, 1.0, 0.0);
    Quat q = Quat::FromToRotation(from, to);
    EXPECT_TRUE(q.IsNormalized());
    EXPECT_NEAR(q.x, -0.11275, 1e-5);
    EXPECT_NEAR(q.y, 0.0, 1e-5);
    EXPECT_NEAR(q.z, 0.22550, 1e-5);
    EXPECT_NEAR(q.w, 0.96770, 1e-5);
}

TYPED_TEST(QuaterniontTest, FromToRotationSameVector)
{
    using Quat = TestFixture::Quat;
    using Vec3 = TestFixture::Vec3;
    Vec3 from(0.0, 0.0, 1.0);
    Vec3 to(0.0, 0.0, 1.0);
    Quat q = Quat::FromToRotation(from, to);
    EXPECT_TRUE(q.IsNormalized());
    EXPECT_NEAR(q.w, 1.0, 1e-5);
}

TYPED_TEST(QuaterniontTest, FromToRotationOppositeVector)
{
    using Quat = TestFixture::Quat;
    using Vec3 = TestFixture::Vec3;
    Vec3 from(0.0, 0.0, 1.0);
    Vec3 to(0.0, 0.0, -1.0);
    Quat q = Quat::FromToRotation(from, to);
    EXPECT_TRUE(q.IsNormalized());
}

TYPED_TEST(QuaterniontTest, FromToRotationPerpendicularVector)
{
    using Quat = TestFixture::Quat;
    using Vec3 = TestFixture::Vec3;
    Vec3 from(1.0, 0.0, 0.0);
    Vec3 to(0.0, 1.0, 0.0);
    Quat q = Quat::FromToRotation(from, to);
    EXPECT_TRUE(q.IsNormalized());
}

TYPED_TEST(QuaterniontTest, FromToRotationZeroVector)
{
    using Quat = TestFixture::Quat;
    using Vec3 = TestFixture::Vec3;
    Vec3 from(0.0, 0.0, 0.0);
    Vec3 to(0.0, 0.0, 0.0);
    Quat q = Quat::FromToRotation(from, to);
    EXPECT_EQ(q.x, 0.0);
    EXPECT_EQ(q.y, 0.0);
    EXPECT_EQ(q.z, 0.0);
    EXPECT_EQ(q.w, 1.0);
}