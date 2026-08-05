#include <gtest/gtest.h>
#include <cmath>
#include "ByteEngine/Math/Quaternion.h"
#include "ByteEngine/Math/Vector3.h"
#include "ByteEngine/Math/Math.h"

using namespace ByteEngine::Math;

class QuaternionTest : public ::testing::Test
{
protected:
    const float EPSILON = 1e-5f;
};

TEST_F(QuaternionTest, LengthSquared)
{
    Quaternion q(1.0f, 2.0f, 2.0f, 0.0f);
    EXPECT_NEAR(q.LengthSquared(), 9.0f, EPSILON);
}

TEST_F(QuaternionTest, Length)
{
    Quaternion q(1.0f, 2.0f, 2.0f, 0.0f);
    EXPECT_NEAR(q.Length(), 3.0f, EPSILON);
}

TEST_F(QuaternionTest, ZeroQuaternionLength)
{
    Quaternion q(0.0f);
    EXPECT_NEAR(q.Length(), 0.0f, EPSILON);
}

TEST_F(QuaternionTest, Normalize)
{
    Quaternion q(1.0f, 2.0f, 2.0f, 0.0f);
    q.Normalize();
    EXPECT_NEAR(q.Length(), 1.0f, EPSILON);
    EXPECT_TRUE(q.IsNormalized());
}

TEST_F(QuaternionTest, Normalized)
{
    Quaternion q(1.0f, 2.0f, 2.0f, 0.0f);
    Quaternion normalized = q.Normalized();
    EXPECT_NEAR(normalized.Length(), 1.0f, EPSILON);
    EXPECT_TRUE(normalized.IsNormalized());
    EXPECT_NEAR(q.Length(), 3.0f, EPSILON);
}

TEST_F(QuaternionTest, IsNormalized)
{
    Quaternion normalized(0.577f, 0.577f, 0.577f, 0.0f);
    normalized.Normalize();
    EXPECT_TRUE(normalized.IsNormalized());

    Quaternion notNormalized(1.0f, 2.0f, 3.0f, 4.0f);
    EXPECT_FALSE(notNormalized.IsNormalized());
}

TEST_F(QuaternionTest, IsEqualApproximetly)
{
    Quaternion q1(1.00001f, 2.00001f, 3.00001f, 4.00001f);
    Quaternion q2(1.00002f, 2.00002f, 3.00002f, 4.00002f);
    EXPECT_TRUE(Quaternion::IsEqualApproximetly(q1, q2, 0.0001f));
    EXPECT_FALSE(Quaternion::IsEqualApproximetly(q1, q2, 0.000001f));
}

TEST_F(QuaternionTest, Inverse)
{
    Quaternion q = Quaternion(0.0f, 0.0f, 1.0f, 0.0f).Normalized();
    q.Inverse();
    EXPECT_NEAR(q.x, 0.0f, EPSILON);
    EXPECT_NEAR(q.y, 0.0f, EPSILON);
    EXPECT_NEAR(q.z, -1.0f, EPSILON);
    EXPECT_NEAR(q.w, 0.0f, EPSILON);
}

TEST_F(QuaternionTest, Inversed)
{
    Quaternion q = Quaternion(0.0f, 0.0f, 1.0f, 0.0f).Normalized();
    Quaternion inversed = q.Inversed();
    EXPECT_NEAR(inversed.x, 0.0f, EPSILON);
    EXPECT_NEAR(inversed.y, 0.0f, EPSILON);
    EXPECT_NEAR(inversed.z, -1.0f, EPSILON);
    EXPECT_NEAR(inversed.w, 0.0f, EPSILON);
    EXPECT_NEAR(q.z, 1.0f, EPSILON);
}

TEST_F(QuaternionTest, FromEulerInRadians)
{
    Quaternion q = Quaternion::FromEuler(0.0_rf, 0.0_rf, 0.0_rf);
    EXPECT_NEAR(q.x, 0.0f, EPSILON);
    EXPECT_NEAR(q.y, 0.0f, EPSILON);
    EXPECT_NEAR(q.z, 0.0f, EPSILON);
    EXPECT_NEAR(q.w, 1.0f, EPSILON);
}

TEST_F(QuaternionTest, FromEulerInDegrees)
{
    Vector3 euler(0.0f, 0.0f, 0.0f);
    Quaternion q = Quaternion::FromEuler(0.0_df, 0.0_df, 0.0_df);
    EXPECT_NEAR(q.x, 0.0f, EPSILON);
    EXPECT_NEAR(q.y, 0.0f, EPSILON);
    EXPECT_NEAR(q.z, 0.0f, EPSILON);
    EXPECT_NEAR(q.w, 1.0f, EPSILON);
}

TEST_F(QuaternionTest, FromEulerRadianComponents)
{
    Quaternion q = Quaternion::FromEuler(RadianF(0.0f), RadianF(0.0f), RadianF(0.0f));
    EXPECT_NEAR(q.x, 0.0f, EPSILON);
    EXPECT_NEAR(q.y, 0.0f, EPSILON);
    EXPECT_NEAR(q.z, 0.0f, EPSILON);
    EXPECT_NEAR(q.w, 1.0f, EPSILON);
}

TEST_F(QuaternionTest, FromEulerDegreeComponents)
{
    Quaternion q = Quaternion::FromEuler(DegreeF(0.0f), DegreeF(0.0f), DegreeF(0.0f));
    EXPECT_NEAR(q.x, 0.0f, EPSILON);
    EXPECT_NEAR(q.y, 0.0f, EPSILON);
    EXPECT_NEAR(q.z, 0.0f, EPSILON);
    EXPECT_NEAR(q.w, 1.0f, EPSILON);
}

TEST_F(QuaternionTest, GetEuler)
{
    Quaternion q = Quaternion::FromEuler(0.0_df, 0.0_df, 0.0_df);
    EulerRad euler = q.GetEuler();
    EXPECT_NEAR(euler.pitch.value, 0.0f, EPSILON);
    EXPECT_NEAR(euler.yaw.value, 0.0f, EPSILON);
    EXPECT_NEAR(euler.roll.value, 0.0f, EPSILON);
}

TEST_F(QuaternionTest, GetEulerInDegrees)
{
    Quaternion q = Quaternion::FromEuler(0.0_df, 0.0_df, 0.0_df);
    EulerDeg euler = q.GetEulerInDegrees();
    EXPECT_NEAR(euler.pitch.value, 0.0f, EPSILON);
    EXPECT_NEAR(euler.yaw.value, 0.0f, EPSILON);
    EXPECT_NEAR(euler.roll.value, 0.0f, EPSILON);
}

TEST_F(QuaternionTest, FromAngleAxisRadian)
{
    Vector3 axis(0.0f, 1.0f, 0.0f);
    RadianF angle(0.0f);
    Quaternion q = Quaternion::FromAngleAxis(angle, axis);
    EXPECT_NEAR(q.x, 0.0f, EPSILON);
    EXPECT_NEAR(q.y, 0.0f, EPSILON);
    EXPECT_NEAR(q.z, 0.0f, EPSILON);
    EXPECT_NEAR(q.w, 1.0f, EPSILON);
}

TEST_F(QuaternionTest, FromAngleAxisDegree)
{
    Vector3 axis(0.0f, 1.0f, 0.0f);
    DegreeF angle(0.0f);
    Quaternion q = Quaternion::FromAngleAxis(angle, axis);
    EXPECT_NEAR(q.x, 0.0f, EPSILON);
    EXPECT_NEAR(q.y, 0.0f, EPSILON);
    EXPECT_NEAR(q.z, 0.0f, EPSILON);
    EXPECT_NEAR(q.w, 1.0f, EPSILON);
}

TEST_F(QuaternionTest, GetAxis)
{
    Quaternion q = Quaternion::FromAngleAxis(RadianF(Mathf::PI / 2.0f), Vector3(0.0f, 1.0f, 0.0f));
    Vector3 axis = q.GetAxis();
    EXPECT_NEAR(axis.x, 0.0f, EPSILON);
    EXPECT_NEAR(std::abs(axis.y), 1.0f, EPSILON);
    EXPECT_NEAR(axis.z, 0.0f, EPSILON);
}

TEST_F(QuaternionTest, GetAngle)
{
    Quaternion q = Quaternion::FromAngleAxis(RadianF(Mathf::PI / 2.0f), Vector3(0.0f, 1.0f, 0.0f));
    RadianF angle = q.GetAngle();
    EXPECT_NEAR(angle.value, Mathf::PI / 2.0f, EPSILON);
}

TEST_F(QuaternionTest, Dot)
{
    Quaternion q1(1.0f, 0.0f, 0.0f, 0.0f);
    Quaternion q2(1.0f, 0.0f, 0.0f, 0.0f);
    EXPECT_NEAR(Quaternion::Dot(q1, q2), 1.0f, EPSILON);

    Quaternion q3(1.0f, 1.0f, 1.0f, 1.0f);
    Quaternion q4(1.0f, 1.0f, 1.0f, 1.0f);
    EXPECT_NEAR(Quaternion::Dot(q3, q4), 4.0f, EPSILON);
}

TEST_F(QuaternionTest, AngleBetween)
{
    Quaternion q1 = Quaternion::FromAngleAxis(RadianF(0.0f), Vector3(0.0f, 1.0f, 0.0f));
    Quaternion q2 = Quaternion::FromAngleAxis(RadianF(0.0f), Vector3(0.0f, 1.0f, 0.0f));
    RadianF angle = Quaternion::AngleBetween(q1, q2);
    EXPECT_NEAR(angle.value, 0.0f, EPSILON);
}

TEST_F(QuaternionTest, UnaryPlus)
{
    Quaternion q(1.0f, -2.0f, 3.0f, -4.0f);
    Quaternion result = +q;
    EXPECT_EQ(result.x, 1.0f);
    EXPECT_EQ(result.y, -2.0f);
    EXPECT_EQ(result.z, 3.0f);
    EXPECT_EQ(result.w, -4.0f);
}

TEST_F(QuaternionTest, UnaryMinus)
{
    Quaternion q(1.0f, -2.0f, 3.0f, -4.0f);
    Quaternion result = -q;
    EXPECT_EQ(result.x, -1.0f);
    EXPECT_EQ(result.y, 2.0f);
    EXPECT_EQ(result.z, -3.0f);
    EXPECT_EQ(result.w, 4.0f);
}

TEST_F(QuaternionTest, Addition)
{
    Quaternion q1(1.0f, 2.0f, 3.0f, 4.0f);
    Quaternion q2(5.0f, 6.0f, 7.0f, 8.0f);
    Quaternion result = q1 + q2;
    EXPECT_EQ(result.x, 6.0f);
    EXPECT_EQ(result.y, 8.0f);
    EXPECT_EQ(result.z, 10.0f);
    EXPECT_EQ(result.w, 12.0f);
}

TEST_F(QuaternionTest, Subtraction)
{
    Quaternion q1(5.0f, 6.0f, 7.0f, 8.0f);
    Quaternion q2(1.0f, 2.0f, 3.0f, 4.0f);
    Quaternion result = q1 - q2;
    EXPECT_EQ(result.x, 4.0f);
    EXPECT_EQ(result.y, 4.0f);
    EXPECT_EQ(result.z, 4.0f);
    EXPECT_EQ(result.w, 4.0f);
}

TEST_F(QuaternionTest, AdditionAssignment)
{
    Quaternion q1(1.0f, 2.0f, 3.0f, 4.0f);
    Quaternion q2(5.0f, 6.0f, 7.0f, 8.0f);
    q1 += q2;
    EXPECT_EQ(q1.x, 6.0f);
    EXPECT_EQ(q1.y, 8.0f);
    EXPECT_EQ(q1.z, 10.0f);
    EXPECT_EQ(q1.w, 12.0f);
}

TEST_F(QuaternionTest, SubtractionAssignment)
{
    Quaternion q1(5.0f, 6.0f, 7.0f, 8.0f);
    Quaternion q2(1.0f, 2.0f, 3.0f, 4.0f);
    q1 -= q2;
    EXPECT_EQ(q1.x, 4.0f);
    EXPECT_EQ(q1.y, 4.0f);
    EXPECT_EQ(q1.z, 4.0f);
    EXPECT_EQ(q1.w, 4.0f);
}

TEST_F(QuaternionTest, Multiplication)
{
    Quaternion q1(1.0f, 0.0f, 0.0f, 0.0f);
    Quaternion q2(0.0f, 1.0f, 0.0f, 0.0f);
    Quaternion result = q1 * q2;
    EXPECT_NEAR(result.x, 0.0f, EPSILON);
    EXPECT_NEAR(result.y, 0.0f, EPSILON);
    EXPECT_NEAR(result.z, 1.0f, EPSILON);
    EXPECT_NEAR(result.w, 0.0f, EPSILON);
}

TEST_F(QuaternionTest, MultiplicationAssignment)
{
    Quaternion q1(1.0f, 0.0f, 0.0f, 0.0f);
    Quaternion q2(0.0f, 1.0f, 0.0f, 0.0f);
    q1 *= q2;
    EXPECT_NEAR(q1.x, 0.0f, EPSILON);
    EXPECT_NEAR(q1.y, 0.0f, EPSILON);
    EXPECT_NEAR(q1.z, 1.0f, EPSILON);
    EXPECT_NEAR(q1.w, 0.0f, EPSILON);
}

TEST_F(QuaternionTest, EqualityOperator)
{
    Quaternion q1(1.0f, 2.0f, 3.0f, 4.0f);
    Quaternion q2(1.0f, 2.0f, 3.0f, 4.0f);
    Quaternion q3(1.0f, 2.0f, 3.0f, 5.0f);

    EXPECT_TRUE(q1 == q2);
    EXPECT_FALSE(q1 == q3);
}

TEST_F(QuaternionTest, InequalityOperator)
{
    Quaternion q1(1.0f, 2.0f, 3.0f, 4.0f);
    Quaternion q2(1.0f, 2.0f, 3.0f, 4.0f);
    Quaternion q3(1.0f, 2.0f, 3.0f, 5.0f);

    EXPECT_FALSE(q1 != q2);
    EXPECT_TRUE(q1 != q3);
}

TEST_F(QuaternionTest, SubscriptOperatorRead)
{
    Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
    EXPECT_EQ(q[0], 1.0f);
    EXPECT_EQ(q[1], 2.0f);
    EXPECT_EQ(q[2], 3.0f);
    EXPECT_EQ(q[3], 4.0f);
}

TEST_F(QuaternionTest, SubscriptOperatorWrite)
{
    Quaternion q(0.0f);
    q[0] = 1.0f;
    q[1] = 2.0f;
    q[2] = 3.0f;
    q[3] = 4.0f;
    EXPECT_EQ(q.x, 1.0f);
    EXPECT_EQ(q.y, 2.0f);
    EXPECT_EQ(q.z, 3.0f);
    EXPECT_EQ(q.w, 4.0f);
}

TEST_F(QuaternionTest, CastToVector3)
{
    Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
    Vector3 v = static_cast<Vector3>(q);
    EXPECT_EQ(v.x, 1.0f);
    EXPECT_EQ(v.y, 2.0f);
    EXPECT_EQ(v.z, 3.0f);
}

TEST_F(QuaternionTest, CastToVector2)
{
    Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
    Vector2 v = static_cast<Vector2>(q);
    EXPECT_EQ(v.x, 1.0f);
    EXPECT_EQ(v.y, 2.0f);
}

TEST_F(QuaternionTest, SlerpAtZero)
{
    Quaternion q1 = Quaternion::FromAngleAxis(RadianF(0.0f), Vector3(0.0f, 1.0f, 0.0f));
    Quaternion q2 = Quaternion::FromAngleAxis(RadianF(Mathf::PI / 2.0f), Vector3(0.0f, 1.0f, 0.0f));
    Quaternion result = Quaternion::Slerp(q1, q2, 0.0f);
    EXPECT_NEAR(result.x, q1.x, EPSILON);
    EXPECT_NEAR(result.y, q1.y, EPSILON);
    EXPECT_NEAR(result.z, q1.z, EPSILON);
    EXPECT_NEAR(result.w, q1.w, EPSILON);
}

TEST_F(QuaternionTest, SlerpAtOne)
{
    Quaternion q1 = Quaternion::FromAngleAxis(RadianF(0.0f), Vector3(0.0f, 1.0f, 0.0f));
    Quaternion q2 = Quaternion::FromAngleAxis(RadianF(Mathf::PI / 2.0f), Vector3(0.0f, 1.0f, 0.0f));
    Quaternion result = Quaternion::Slerp(q1, q2, 1.0f);
    EXPECT_NEAR(result.x, q2.x, EPSILON);
    EXPECT_NEAR(result.y, q2.y, EPSILON);
    EXPECT_NEAR(result.z, q2.z, EPSILON);
    EXPECT_NEAR(result.w, q2.w, EPSILON);
}

TEST_F(QuaternionTest, SlerpClampedAtZero)
{
    Quaternion q1 = Quaternion::FromAngleAxis(RadianF(0.0f), Vector3(0.0f, 1.0f, 0.0f));
    Quaternion q2 = Quaternion::FromAngleAxis(RadianF(Mathf::PI / 2.0f), Vector3(0.0f, 1.0f, 0.0f));
    Quaternion result = Quaternion::SlerpClamped(q1, q2, 0.0f);
    EXPECT_NEAR(result.x, q1.x, EPSILON);
    EXPECT_NEAR(result.y, q1.y, EPSILON);
    EXPECT_NEAR(result.z, q1.z, EPSILON);
    EXPECT_NEAR(result.w, q1.w, EPSILON);
}

TEST_F(QuaternionTest, SlerpClampedAtOne)
{
    Quaternion q1 = Quaternion::FromAngleAxis(RadianF(0.0f), Vector3(0.0f, 1.0f, 0.0f));
    Quaternion q2 = Quaternion::FromAngleAxis(RadianF(Mathf::PI / 2.0f), Vector3(0.0f, 1.0f, 0.0f));
    Quaternion result = Quaternion::SlerpClamped(q1, q2, 1.0f);
    EXPECT_NEAR(result.x, q2.x, EPSILON);
    EXPECT_NEAR(result.y, q2.y, EPSILON);
    EXPECT_NEAR(result.z, q2.z, EPSILON);
    EXPECT_NEAR(result.w, q2.w, EPSILON);
}

TEST_F(QuaternionTest, SlerpClampedClamps)
{
    Quaternion q1 = Quaternion::FromAngleAxis(RadianF(0.0f), Vector3(0.0f, 1.0f, 0.0f));
    Quaternion q2 = Quaternion::FromAngleAxis(RadianF(Mathf::PI / 2.0f), Vector3(0.0f, 1.0f, 0.0f));
    Quaternion result = Quaternion::SlerpClamped(q1, q2, 1.5f);
    EXPECT_NEAR(result.x, q2.x, EPSILON);
    EXPECT_NEAR(result.y, q2.y, EPSILON);
    EXPECT_NEAR(result.z, q2.z, EPSILON);
    EXPECT_NEAR(result.w, q2.w, EPSILON);
}

TEST_F(QuaternionTest, FromLookDirectionAlongZ)
{
    Vector3 direction(0.0f, 0.0f, 1.0f);
    Vector3 worldUp(0.0f, 1.0f, 0.0f);
    Quaternion q = Quaternion::FromLookDirection(direction, worldUp);
    EXPECT_TRUE(q.IsNormalized());
}

TEST_F(QuaternionTest, FromLookDirectionZeroDirection)
{
    Vector3 direction(0.0f, 0.0f, 0.0f);
    Vector3 worldUp(0.0f, 1.0f, 0.0f);
    Quaternion q = Quaternion::FromLookDirection(direction, worldUp);
    EXPECT_EQ(q.x, 0.0f);
    EXPECT_EQ(q.y, 0.0f);
    EXPECT_EQ(q.z, 0.0f);
    EXPECT_EQ(q.w, 1.0f);
}

TEST_F(QuaternionTest, FromToRotation)
{
    Vector3 from(1.0f, 2.0f, 0.5f);
    Vector3 to(0.0f, 1.0f, 0.0f);
    Quaternion q = Quaternion::FromToRotation(from, to);
    EXPECT_TRUE(q.IsNormalized());
    EXPECT_NEAR(q.x, -0.11275f, EPSILON);
    EXPECT_NEAR(q.y, 0.0f, EPSILON);
    EXPECT_NEAR(q.z, 0.22550f, EPSILON);
    EXPECT_NEAR(q.w, 0.96770f, EPSILON);
}

TEST_F(QuaternionTest, FromToRotationSameVector)
{
    Vector3 from(0.0f, 0.0f, 1.0f);
    Vector3 to(0.0f, 0.0f, 1.0f);
    Quaternion q = Quaternion::FromToRotation(from, to);
    EXPECT_TRUE(q.IsNormalized());
    EXPECT_NEAR(q.w, 1.0f, EPSILON);
}

TEST_F(QuaternionTest, FromToRotationOppositeVector)
{
    Vector3 from(0.0f, 0.0f, 1.0f);
    Vector3 to(0.0f, 0.0f, -1.0f);
    Quaternion q = Quaternion::FromToRotation(from, to);
    EXPECT_TRUE(q.IsNormalized());
}

TEST_F(QuaternionTest, FromToRotationPerpendicularVector)
{
    Vector3 from(1.0f, 0.0f, 0.0f);
    Vector3 to(0.0f, 1.0f, 0.0f);
    Quaternion q = Quaternion::FromToRotation(from, to);
    EXPECT_TRUE(q.IsNormalized());
}

TEST_F(QuaternionTest, FromToRotationZeroVector)
{
    Vector3 from(0.0f, 0.0f, 0.0f);
    Vector3 to(0.0f, 0.0f, 0.0f);
    Quaternion q = Quaternion::FromToRotation(from, to);
    EXPECT_EQ(q.x, 0.0f);
    EXPECT_EQ(q.y, 0.0f);
    EXPECT_EQ(q.z, 0.0f);
    EXPECT_EQ(q.w, 1.0f);
}