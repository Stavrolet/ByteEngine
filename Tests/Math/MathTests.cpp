#include "ByteEngine/Math/Math.h"

#include <gtest/gtest.h>

using namespace ByteEngine::Math;

TEST(MathTest, ConstantsAndConversions)
{
    EXPECT_NEAR(static_cast<RadianF>(180_df).value, Mathf::PI, 1e-5f);
    EXPECT_NEAR(RadianD(Mathf::PI_D).ToDegree().value, 180.0, 1e-5);
}

TEST(MathTest, Trigonometry)
{
    EXPECT_NEAR(Mathf::Sin(0.0_rf), 0.0f, 1e-5f);
    EXPECT_NEAR(Mathf::Sin(RadianF(Mathf::PI / 2.0f)), 1.0f, 1e-5f);
    EXPECT_NEAR(Mathf::Cos(0.0_rf), 1.0f, 1e-5f);
    EXPECT_NEAR(Mathf::Cos(RadianF(Mathf::PI)), -1.0f, 1e-5f);

    float s, c;
    Mathf::SinCos(s, c, RadianF(Mathf::PI / 2.0f));
    EXPECT_NEAR(s, 1.0f, 1e-5f);
    EXPECT_NEAR(c, 0.0f, 1e-5f);

    EXPECT_NEAR(Mathf::Asin(1.0f).value, Mathf::PI / 2.0f, 1e-4f);
    EXPECT_NEAR(Mathf::Acos(0.0f).value, Mathf::PI / 2.0f, 1e-4f);
}

TEST(MathTest, BasicUtilities)
{
    EXPECT_EQ(Mathf::Sign(-10.0f), -1.0f);
    EXPECT_EQ(Mathf::Sign(10.0f), 1.0f);
    EXPECT_EQ(Mathf::Sign(0.0f), 0.0f);

    EXPECT_NEAR(Mathf::Fract(1.75f), 0.75f, 1e-5f);
}

TEST(MathTest, Algebra)
{
    EXPECT_NEAR(Mathf::LogN(25.0f, 5.0f), 2.0f, 1e-5);
}

TEST(MathTest, ClampAndRemap)
{
    EXPECT_EQ(Mathf::Clamp(0.5f, 0.0f, 1.0f), 0.5f);
    EXPECT_EQ(Mathf::Clamp(2.0f, 0.0f, 1.0f), 1.0f);
    EXPECT_EQ(Mathf::Clamp(-1.0f, 0.0f, 1.0f), 0.0f);

    EXPECT_NEAR(Mathf::Remap(0.5f, 0.0f, 1.0f, 0.0f, 10.0f), 5.0f, 1e-5f);
}

TEST(MathTest, AngleDifference)
{
    EXPECT_NEAR(Mathf::AngleDifference(18.8495559_rf, RadianF(Mathf::PI / 2)).value, Mathf::PI / 2, 1e-4f);
}

TEST(MathTest, Interpolation)
{
    EXPECT_NEAR(Mathf::Lerp(0.0f, 10.0f, 0.5f), 5.0f, 1e-5f);
    EXPECT_NEAR(Mathf::InverseLerp(0.0f, 10.0f, 5.0f), 0.5f, 1e-5f);

    EXPECT_NEAR(Mathf::LerpAngle(0.0_rf, Mathf::PI / 2.0_rf + Mathf::PI * 2_rf, 0.5f).value, 0.7853981f, 1e-4f);

    EXPECT_NEAR(Mathf::MoveTowards(0.0f, 10.0f, 2.0f), 2.0f, 1e-5f);
    EXPECT_NEAR(Mathf::MoveTowards(0.0f, 1.0f, 2.0f), 1.0f, 1e-5f);

    EXPECT_NEAR(Mathf::SmoothStep(0, 10, 2.0f), 0.104f, 1e-4f);
    EXPECT_NEAR(Mathf::SmoothStep(10, 10.0f, 0.3), 0.0, 1e-4f);
}

TEST(MathTest, RangeFunctions)
{
    EXPECT_NEAR(Mathf::WrapValue(12.0, 0.0f, 10.0f), 2.0f, 1e-5f);
    EXPECT_NEAR(Mathf::WrapValue(-2.0f, 0.0f, 10.0f), 8.0f, 1e-5f);

    EXPECT_EQ(Mathf::WrapValue(12, 0, 10), 2);
    EXPECT_EQ(Mathf::WrapValue(-2, 0, 10ll), 8);

    EXPECT_NEAR(Mathf::PingPong(12.0f, 10.0f), 8.0f, 1e-5f);
}

TEST(MathTest, EqualApproximetly)
{
    EXPECT_TRUE(Mathf::IsEqualApproximetly(1.0000001f, 1.0f, 0.0001f));
}

TEST(MathTest, Average)
{
    std::vector<float> data = { 1.0f, 2.0f, 3.0f, 4.0f };
    EXPECT_NEAR(Mathf::Average(data), 2.5f, 1e-5f);

    EXPECT_NEAR(Mathf::Average<double>({ 10.0, 20.0 }), 15.0, 1e-5);
}

TEST(MathTest, MinMax)
{
    std::vector<int> data = { 1, 2, 3, 4, -100, 0 };

    EXPECT_EQ(Mathf::Min(10, 2), 2);
    EXPECT_EQ(Mathf::Min(10, 2, 100), 2);
    EXPECT_EQ(Mathf::Min(data), -100);

    EXPECT_EQ(Mathf::Max(10, 2), 10);
    EXPECT_EQ(Mathf::Max(10, 2, 100), 100);
    EXPECT_EQ(Mathf::Max(data), 4);
}