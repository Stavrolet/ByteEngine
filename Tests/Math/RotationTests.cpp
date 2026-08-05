#include <gtest/gtest.h>
#include <cmath>
#include "ByteEngine/Math/Rotation.h"
#include "ByteEngine/Math/Quaternion.h"

using namespace ByteEngine::Math;
using namespace ByteEngine::Math::Literals;

// ─────────────────────────────────────────────
// Helpers
// ─────────────────────────────────────────────

static constexpr float kEps = 1e-4f;
static constexpr float kLooseEps = 1e-2f; // for Euler↔Quaternion round-trips

static bool RotEqual(const Rotation& a, const Rotation& b, float eps = kEps)
{
    return std::fabs(a.pitch.value - b.pitch.value) < eps &&
        std::fabs(a.yaw.value - b.yaw.value) < eps &&
        std::fabs(a.roll.value - b.roll.value) < eps;
}

// ─────────────────────────────────────────────
// Construction
// ─────────────────────────────────────────────

TEST(RotationConstructionTest, DefaultConstructorIsZero)
{
    Rotation r;
    EXPECT_EQ(r.pitch.value, 0.0f);
    EXPECT_EQ(r.yaw.value, 0.0f);
    EXPECT_EQ(r.roll.value, 0.0f);
}

TEST(RotationConstructionTest, DegreeConstructor)
{
    Rotation r(45.0_df, 90.0_df, 180.0_df);
    EXPECT_NEAR(r.pitch.value, 45.0f, kEps);
    EXPECT_NEAR(r.yaw.value, 90.0f, kEps);
    EXPECT_NEAR(r.roll.value, 180.0f, kEps);
}

TEST(RotationConstructionTest, RadianConstructorConvertsCorrectly)
{
    // PI/2 rad == 90 degrees
    Rotation r(RadianF(Mathf::PI / 2.f), RadianF(Mathf::PI), RadianF(0.f));
    EXPECT_NEAR(r.pitch.value, 90.0f, kLooseEps);
    EXPECT_NEAR(r.yaw.value, 180.0f, kLooseEps);
    EXPECT_NEAR(r.roll.value, 0.0f, kLooseEps);
}

TEST(RotationConstructionTest, DegreeArrayConstructor)
{
    DegreeF arr[3] = { 10.0_df, 20.0_df, 30.0_df };
    Rotation r(arr);
    EXPECT_NEAR(r.pitch.value, 10.0f, kEps);
    EXPECT_NEAR(r.yaw.value, 20.0f, kEps);
    EXPECT_NEAR(r.roll.value, 30.0f, kEps);
}

TEST(RotationConstructionTest, RadianArrayConstructor)
{
    RadianF arr[3] = { RadianF(0.f), RadianF(Mathf::PI / 2.f), RadianF(Mathf::PI) };
    Rotation r(arr);
    EXPECT_NEAR(r.pitch.value, 0.0f, kLooseEps);
    EXPECT_NEAR(r.yaw.value, 90.0f, kLooseEps);
    EXPECT_NEAR(r.roll.value, 180.0f, kLooseEps);
}

TEST(RotationConstructionTest, EulerDegConstructor)
{
    EulerDeg euler { 15.0_df, 30.0_df, 45.0_df };
    Rotation r(euler);
    EXPECT_NEAR(r.pitch.value, 15.0f, kEps);
    EXPECT_NEAR(r.yaw.value, 30.0f, kEps);
    EXPECT_NEAR(r.roll.value, 45.0f, kEps);
}

TEST(RotationConstructionTest, EulerRadConstructor)
{
    EulerRad euler { RadianF(0.f), RadianF(Mathf::PI / 2.f), RadianF(Mathf::PI) };
    Rotation r(euler);
    EXPECT_NEAR(r.pitch.value, 0.0f, kLooseEps);
    EXPECT_NEAR(r.yaw.value, 90.0f, kLooseEps);
    EXPECT_NEAR(r.roll.value, 180.0f, kLooseEps);
}

TEST(RotationConstructionTest, QuaternionConstructorIdentityGivesZero)
{
    Rotation r(Quaternion::Identity);
    EXPECT_NEAR(r.pitch.value, 0.0f, kLooseEps);
    EXPECT_NEAR(r.yaw.value, 0.0f, kLooseEps);
    EXPECT_NEAR(r.roll.value, 0.0f, kLooseEps);
}

TEST(RotationConstructionTest, QuaternionConstructorRoundTrip)
{
    Rotation original(30.0_df, 45.0_df, 60.0_df);
    Quaternion q = original.ToQuaternion();
    Rotation roundTripped(q);
    EXPECT_TRUE(RotEqual(original, roundTripped, kLooseEps));
}

// ─────────────────────────────────────────────
// Accessors
// ─────────────────────────────────────────────

TEST(RotationAccessTest, IndexOperatorRead)
{
    Rotation r(10.0_df, 20.0_df, 30.0_df);
    EXPECT_NEAR(r[0].value, 10.0f, kEps);
    EXPECT_NEAR(r[1].value, 20.0f, kEps);
    EXPECT_NEAR(r[2].value, 30.0f, kEps);
}

TEST(RotationAccessTest, IndexOperatorWrite)
{
    Rotation r;
    r[0] = 99.0_df;
    r[1] = -45.0_df;
    r[2] = 180.0_df;
    EXPECT_NEAR(r.pitch.value, 99.0f, kEps);
    EXPECT_NEAR(r.yaw.value, -45.0f, kEps);
    EXPECT_NEAR(r.roll.value, 180.0f, kEps);
}

TEST(RotationAccessTest, IndexAndNamedFieldsAliasToSameMemory)
{
    Rotation r(1.0_df, 2.0_df, 3.0_df);
    EXPECT_EQ(r[0].value, r.pitch.value);
    EXPECT_EQ(r[1].value, r.yaw.value);
    EXPECT_EQ(r[2].value, r.roll.value);
}

// ─────────────────────────────────────────────
// Equality operators
// ─────────────────────────────────────────────

TEST(RotationOperatorTest, EqualityIdentical)
{
    Rotation a(10.0_df, 20.0_df, 30.0_df);
    Rotation b(10.0_df, 20.0_df, 30.0_df);
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
}

TEST(RotationOperatorTest, InequalityDifferent)
{
    Rotation a(10.0_df, 20.0_df, 30.0_df);
    Rotation b(10.0_df, 20.0_df, 31.0_df);
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
}

TEST(RotationOperatorTest, EqualitySymmetric)
{
    Rotation a(45.0_df, 0.0_df, -90.0_df);
    Rotation b(45.0_df, 0.0_df, -90.0_df);
    EXPECT_TRUE(a == b);
    EXPECT_TRUE(b == a);
}

TEST(RotationOperatorTest, DefaultConstructedRotationsAreEqual)
{
    Rotation a;
    Rotation b;
    EXPECT_TRUE(a == b);
}

// ─────────────────────────────────────────────
// Arithmetic operators
// ─────────────────────────────────────────────

TEST(RotationOperatorTest, UnaryPlus)
{
    Rotation r(10.0_df, -20.0_df, 30.0_df);
    Rotation p = +r;
    EXPECT_TRUE(p == r);
}

TEST(RotationOperatorTest, UnaryNegate)
{
    Rotation r(10.0_df, -20.0_df, 30.0_df);
    Rotation n = -r;
    EXPECT_NEAR(n.pitch.value, -10.0f, kEps);
    EXPECT_NEAR(n.yaw.value, 20.0f, kEps);
    EXPECT_NEAR(n.roll.value, -30.0f, kEps);
}

TEST(RotationOperatorTest, Addition)
{
    Rotation a(10.0_df, 20.0_df, 30.0_df);
    Rotation b(5.0_df, -5.0_df, 15.0_df);
    Rotation c = a + b;
    EXPECT_NEAR(c.pitch.value, 15.0f, kEps);
    EXPECT_NEAR(c.yaw.value, 15.0f, kEps);
    EXPECT_NEAR(c.roll.value, 45.0f, kEps);
}

TEST(RotationOperatorTest, Subtraction)
{
    Rotation a(30.0_df, 60.0_df, 90.0_df);
    Rotation b(10.0_df, 10.0_df, 10.0_df);
    Rotation c = a - b;
    EXPECT_NEAR(c.pitch.value, 20.0f, kEps);
    EXPECT_NEAR(c.yaw.value, 50.0f, kEps);
    EXPECT_NEAR(c.roll.value, 80.0f, kEps);
}

TEST(RotationOperatorTest, AdditionAssignment)
{
    Rotation a(10.0_df, 10.0_df, 10.0_df);
    Rotation b(5.0_df, 5.0_df, 5.0_df);
    a += b;
    EXPECT_NEAR(a.pitch.value, 15.0f, kEps);
    EXPECT_NEAR(a.yaw.value, 15.0f, kEps);
    EXPECT_NEAR(a.roll.value, 15.0f, kEps);
}

TEST(RotationOperatorTest, SubtractionAssignment)
{
    Rotation a(20.0_df, 30.0_df, 40.0_df);
    Rotation b(5.0_df, 5.0_df, 5.0_df);
    a -= b;
    EXPECT_NEAR(a.pitch.value, 15.0f, kEps);
    EXPECT_NEAR(a.yaw.value, 25.0f, kEps);
    EXPECT_NEAR(a.roll.value, 35.0f, kEps);
}

TEST(RotationOperatorTest, ScalarMultiplication)
{
    Rotation r(10.0_df, 20.0_df, 30.0_df);
    Rotation s = r * 2.f;
    EXPECT_NEAR(s.pitch.value, 20.0f, kEps);
    EXPECT_NEAR(s.yaw.value, 40.0f, kEps);
    EXPECT_NEAR(s.roll.value, 60.0f, kEps);
}

TEST(RotationOperatorTest, ScalarMultiplicationCommutative)
{
    Rotation r(10.0_df, 20.0_df, 30.0_df);
    Rotation a = r * 3.f;
    Rotation b = 3.f * r;
    EXPECT_TRUE(RotEqual(a, b));
}

TEST(RotationOperatorTest, ScalarMultiplicationAssignment)
{
    Rotation r(10.0_df, 20.0_df, 30.0_df);
    r *= 2.f;
    EXPECT_NEAR(r.pitch.value, 20.0f, kEps);
    EXPECT_NEAR(r.yaw.value, 40.0f, kEps);
    EXPECT_NEAR(r.roll.value, 60.0f, kEps);
}

TEST(RotationOperatorTest, ScalarDivision)
{
    Rotation r(20.0_df, 40.0_df, 60.0_df);
    Rotation s = r / 2.f;
    EXPECT_NEAR(s.pitch.value, 10.0f, kEps);
    EXPECT_NEAR(s.yaw.value, 20.0f, kEps);
    EXPECT_NEAR(s.roll.value, 30.0f, kEps);
}

TEST(RotationOperatorTest, ScalarDivisionAssignment)
{
    Rotation r(20.0_df, 40.0_df, 60.0_df);
    r /= 2.f;
    EXPECT_NEAR(r.pitch.value, 10.0f, kEps);
    EXPECT_NEAR(r.yaw.value, 20.0f, kEps);
    EXPECT_NEAR(r.roll.value, 30.0f, kEps);
}

TEST(RotationOperatorTest, AddSubtractRoundTrip)
{
    Rotation a(45.0_df, 90.0_df, 135.0_df);
    Rotation b(15.0_df, -30.0_df, 45.0_df);
    Rotation result = (a + b) - b;
    EXPECT_TRUE(RotEqual(result, a));
}

// ─────────────────────────────────────────────
// Normalize
// ─────────────────────────────────────────────

TEST(RotationNormalizeTest, NormalizePositiveOutOfRange)
{
    // 400° should normalize to 40°
    Rotation r(400.0_df, 0.0_df, 0.0_df);
    r.Normalize();
    EXPECT_NEAR(r.pitch.value, 40.0f, kEps);
}

TEST(RotationNormalizeTest, NormalizeNegativeOutOfRange)
{
    // -90° should normalize to -90° (already in (-180, 180])
    Rotation r(-90.0_df, 0.0_df, 0.0_df);
    r.Normalize();
    EXPECT_NEAR(r.pitch.value, -90.0f, kEps);
}

TEST(RotationNormalizeTest, NormalizeValueAbove180)
{
    // 270° should normalize to -90°
    Rotation r(270.0_df, 0.0_df, 0.0_df);
    r.Normalize();
    EXPECT_NEAR(r.pitch.value, -90.0f, kEps);
}

TEST(RotationNormalizeTest, NormalizeAllComponentsIndependently)
{
    Rotation r(370.0_df, -370.0_df, 540.0_df);
    r.Normalize();
    EXPECT_NEAR(r.pitch.value, 10.0f, kEps);  // 370 - 360 = 10
    EXPECT_NEAR(r.yaw.value, -10.0f, kEps);  // -370 + 360 = -10
    EXPECT_NEAR(r.roll.value, -180.0f, kEps);  // 540 - 360 = 180, but 180 > 180 so -180
}

TEST(RotationNormalizeTest, NormalizeZeroRemainsZero)
{
    Rotation r(0.0_df, 0.0_df, 0.0_df);
    r.Normalize();
    EXPECT_NEAR(r.pitch.value, 0.0f, kEps);
    EXPECT_NEAR(r.yaw.value, 0.0f, kEps);
    EXPECT_NEAR(r.roll.value, 0.0f, kEps);
}

TEST(RotationNormalizeTest, NormalizedDoesNotMutate)
{
    Rotation original(400.0_df, 0.0_df, 0.0_df);
    Rotation normalized = original.Normalized();
    // original unchanged
    EXPECT_NEAR(original.pitch.value, 400.0f, kEps);
    // result is normalized
    EXPECT_NEAR(normalized.pitch.value, 40.0f, kEps);
}

TEST(RotationNormalizeTest, NormalizedAndNormalizeMutateProduceSameResult)
{
    Rotation a(730.0_df, -200.0_df, 270.0_df);
    Rotation b = a.Normalized();
    a.Normalize();
    EXPECT_TRUE(RotEqual(a, b));
}

// ─────────────────────────────────────────────
// Conversion: ToQuaternion / FromQuaternion
// ─────────────────────────────────────────────

TEST(RotationConversionTest, ZeroRotationToQuaternionIsIdentity)
{
    Rotation r;
    Quaternion q = r.ToQuaternion();
    EXPECT_NEAR(q.x, Quaternion::Identity.x, kLooseEps);
    EXPECT_NEAR(q.y, Quaternion::Identity.y, kLooseEps);
    EXPECT_NEAR(q.z, Quaternion::Identity.z, kLooseEps);
    EXPECT_NEAR(q.w, Quaternion::Identity.w, kLooseEps);
}

TEST(RotationConversionTest, ToQuaternionIsNormalized)
{
    Rotation r(30.0_df, 45.0_df, 60.0_df);
    Quaternion q = r.ToQuaternion();
    EXPECT_TRUE(q.IsNormalized());
}

TEST(RotationConversionTest, ToQuaternionRoundTrip)
{
    Rotation original(30.0_df, 45.0_df, 0.0_df);
    Quaternion q = original.ToQuaternion();
    Rotation roundTripped = Rotation::FromQuaternion(q);
    EXPECT_TRUE(RotEqual(original, roundTripped, kLooseEps));
}

TEST(RotationConversionTest, FromQuaternionRoundTrip)
{
    Quaternion q = Quaternion::FromAngleAxis(45.0_df, Vector3F::Up());
    Rotation r = Rotation::FromQuaternion(q);
    Quaternion qBack = r.ToQuaternion();
    // Quaternions q and qBack should represent the same rotation.
    // The dot product of two equivalent (unit) quaternions is ±1.
    float dot = std::fabs(Quaternion::Dot(q, qBack));
    EXPECT_NEAR(dot, 1.0f, kLooseEps);
}

TEST(RotationConversionTest, ToEulerDeg)
{
    Rotation r(10.0_df, 20.0_df, 30.0_df);
    EulerDeg euler = r.ToEulerDeg();
    EXPECT_NEAR(euler.pitch.value, 10.0f, kEps);
    EXPECT_NEAR(euler.yaw.value, 20.0f, kEps);
    EXPECT_NEAR(euler.roll.value, 30.0f, kEps);
}

TEST(RotationConversionTest, ToEulerRad)
{
    Rotation r(0.0_df, 90.0_df, 180.0_df);
    EulerRad euler = r.ToEulerRad();
    EXPECT_NEAR(euler.pitch.value, 0.0f, kLooseEps);
    EXPECT_NEAR(euler.yaw.value, Mathf::PI / 2.f, kLooseEps);
    EXPECT_NEAR(euler.roll.value, Mathf::PI, kLooseEps);
}

// ─────────────────────────────────────────────
// IsEqualApproximately
// ─────────────────────────────────────────────

TEST(RotationApproxTest, ExactEqualIsApproxEqual)
{
    Rotation a(45.0_df, 90.0_df, 135.0_df);
    Rotation b(45.0_df, 90.0_df, 135.0_df);
    EXPECT_TRUE(Rotation::IsEqualApproximately(a, b));
}

TEST(RotationApproxTest, SlightlyDifferentIsApproxEqual)
{
    Rotation a(45.0_df, 90.0_df, 135.0_df);
    Rotation b(DegreeF { 45.0f + 0.0001f }, 90.0_df, 135.0_df);
    EXPECT_TRUE(Rotation::IsEqualApproximately(a, b));
}

TEST(RotationApproxTest, LargelyDifferentIsNotApproxEqual)
{
    Rotation a(45.0_df, 90.0_df, 135.0_df);
    Rotation b(46.0_df, 90.0_df, 135.0_df);
    EXPECT_FALSE(Rotation::IsEqualApproximately(a, b));
}

TEST(RotationApproxTest, CustomEpsilonRespected)
{
    Rotation a(45.0_df, 0.0_df, 0.0_df);
    Rotation b(46.0_df, 0.0_df, 0.0_df);
    EXPECT_FALSE(Rotation::IsEqualApproximately(a, b, 0.5_df));
    EXPECT_TRUE(Rotation::IsEqualApproximately(a, b, 2.0_df));
}

// ─────────────────────────────────────────────
// Lerp / LerpClamped
// ─────────────────────────────────────────────

TEST(RotationLerpTest, LerpAtZeroReturnsFrom)
{
    Rotation from(0.0_df, 0.0_df, 0.0_df);
    Rotation to(90.0_df, 90.0_df, 90.0_df);
    Rotation result = Rotation::Lerp(from, to, 0.f);
    EXPECT_TRUE(RotEqual(result, from));
}

TEST(RotationLerpTest, LerpAtOneReturnsTo)
{
    Rotation from(0.0_df, 0.0_df, 0.0_df);
    Rotation to(90.0_df, 90.0_df, 90.0_df);
    Rotation result = Rotation::Lerp(from, to, 1.f);
    EXPECT_TRUE(RotEqual(result, to));
}

TEST(RotationLerpTest, LerpAtHalfReturnsMidpoint)
{
    Rotation from(0.0_df, 0.0_df, 0.0_df);
    Rotation to(90.0_df, 60.0_df, 30.0_df);
    Rotation mid = Rotation::Lerp(from, to, 0.5f);
    EXPECT_NEAR(mid.pitch.value, 45.0f, kEps);
    EXPECT_NEAR(mid.yaw.value, 30.0f, kEps);
    EXPECT_NEAR(mid.roll.value, 15.0f, kEps);
}

TEST(RotationLerpTest, LerpUnclampedExtrapolates)
{
    Rotation from(0.0_df, 0.0_df, 0.0_df);
    Rotation to(10.0_df, 10.0_df, 10.0_df);
    Rotation result = Rotation::Lerp(from, to, 2.f);
    // Lerp is unclamped, so t=2 gives 20°
    EXPECT_NEAR(result.pitch.value, 20.0f, kEps);
}

TEST(RotationLerpTest, LerpClampedClampsAtOne)
{
    Rotation from(0.0_df, 0.0_df, 0.0_df);
    Rotation to(10.0_df, 10.0_df, 10.0_df);
    Rotation result = Rotation::LerpClamped(from, to, 2.f);
    EXPECT_TRUE(RotEqual(result, to));
}

TEST(RotationLerpTest, LerpClampedClampsAtZero)
{
    Rotation from(0.0_df, 0.0_df, 0.0_df);
    Rotation to(10.0_df, 10.0_df, 10.0_df);
    Rotation result = Rotation::LerpClamped(from, to, -1.f);
    EXPECT_TRUE(RotEqual(result, from));
}

// ─────────────────────────────────────────────
// Slerp
// ─────────────────────────────────────────────

TEST(RotationSlerpTest, SlerpAtZeroReturnsFrom)
{
    Rotation from(0.0_df, 0.0_df, 0.0_df);
    Rotation to(90.0_df, 0.0_df, 0.0_df);
    Rotation result = Rotation::Slerp(from, to, 0.f);
    EXPECT_TRUE(RotEqual(result, from, kLooseEps));
}

TEST(RotationSlerpTest, SlerpAtOneReturnsTo)
{
    Rotation from(0.0_df, 0.0_df, 0.0_df);
    Rotation to(90.0_df, 0.0_df, 0.0_df);
    Rotation result = Rotation::Slerp(from, to, 1.f);
    EXPECT_TRUE(RotEqual(result, to, 0.5f));
}

TEST(RotationSlerpTest, SlerpAtHalfIsHalfway)
{
    Rotation from(0.0_df, 0.0_df, 0.0_df);
    Rotation to(90.0_df, 0.0_df, 0.0_df);
    Rotation mid = Rotation::Slerp(from, to, 0.5f);
    EXPECT_NEAR(mid.pitch.value, 45.0f, kLooseEps);
}

TEST(RotationSlerpTest, SlerpResultIsNormalized)
{
    Rotation from(10.0_df, 20.0_df, 30.0_df);
    Rotation to(80.0_df, 70.0_df, 60.0_df);
    Rotation mid = Rotation::Slerp(from, to, 0.5f);
    Quaternion q = mid.ToQuaternion();
    EXPECT_TRUE(q.IsNormalized());
}

TEST(RotationSlerpTest, SlerpIdenticalRotationsReturnSame)
{
    Rotation r(45.0_df, 45.0_df, 45.0_df);
    Rotation result = Rotation::Slerp(r, r, 0.5f);
    EXPECT_TRUE(RotEqual(result, r, kLooseEps));
}