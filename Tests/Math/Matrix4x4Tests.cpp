#include "ByteEngine/Math/Matrix4x4.h"
#include "ByteEngine/Math/Quaternion.h"
#include "ByteEngine/Math/Vector3.h"
#include "ByteEngine/Math/Vector4.h"

#include <cmath>
#include <gtest/gtest.h>

using namespace ByteEngine::Math;
using namespace ByteEngine::Math::Literals;

// ─────────────────────────────────────────────
// Helpers
// ─────────────────────────────────────────────

template <typename T>
static bool Mat4Equal(const Matrix4x4T<T>& a, const Matrix4x4T<T>& b, T eps = static_cast<T>(1e-4))
{
    for (int i = 0; i < Matrix4x4T<T>::ELEMENT_COUNT; ++i)
        if (std::fabs(a.elements[i] - b.elements[i]) > eps)
            return false;
    return true;
}

template <typename T>
static bool Vec3Equal(const Vector3T<T>& a, const Vector3T<T>& b, T eps = static_cast<T>(1e-4))
{
    return std::fabs(a.x - b.x) < eps &&
        std::fabs(a.y - b.y) < eps &&
        std::fabs(a.z - b.z) < eps;
}

template <typename T>
class Matrix4x4tTest : public testing::Test
{
protected:
    using Mat4 = Matrix4x4T<T>;
    using Vec3 = Vector3T<T>;
    using Vec4 = Vector4T<T>;
    using Quat = QuaternionT<T>;
    using RadianT = RadianT<T>;
    using DegreeT = DegreeT<T>;
};

using FloatTypes = testing::Types<float, double>;
TYPED_TEST_SUITE(Matrix4x4tTest, FloatTypes);

// ─────────────────────────────────────────────
// Row / Column accessors
// ─────────────────────────────────────────────

TYPED_TEST(Matrix4x4tTest, GetRow)
{
    using Mat4 = TestFixture::Mat4;
    using Vec4 = TestFixture::Vec4;
    Mat4 m(
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16
    );
    Vec4 row1 = m.GetRow(1);
    EXPECT_EQ(row1.x, 5); EXPECT_EQ(row1.y, 6);
    EXPECT_EQ(row1.z, 7); EXPECT_EQ(row1.w, 8);
}

TYPED_TEST(Matrix4x4tTest, GetRowAllRows)
{
    using Mat4 = TestFixture::Mat4;
    using Vec4 = TestFixture::Vec4;
    Mat4 m(
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16
    );
    for (int r = 0; r < 4; ++r)
    {
        Vec4 row = m.GetRow(r);
        for (int c = 0; c < 4; ++c)
            EXPECT_EQ(row[c], static_cast<TypeParam>(r * 4 + c + 1));
    }
}

TYPED_TEST(Matrix4x4tTest, GetColumn)
{
    using Mat4 = TestFixture::Mat4;
    using Vec4 = TestFixture::Vec4;
    Mat4 m(
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16
    );
    Vec4 col2 = m.GetColumn(2);
    EXPECT_EQ(col2.x, 3);
    EXPECT_EQ(col2.y, 7);
    EXPECT_EQ(col2.z, 11);
    EXPECT_EQ(col2.w, 15);
}

TYPED_TEST(Matrix4x4tTest, GetColumnAllColumns)
{
    using Mat4 = TestFixture::Mat4;
    using Vec4 = TestFixture::Vec4;
    Mat4 m(
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16
    );
    for (int c = 0; c < 4; ++c)
    {
        Vec4 col = m.GetColumn(c);
        EXPECT_EQ(col.x, static_cast<TypeParam>(c + 1));
        EXPECT_EQ(col.y, static_cast<TypeParam>(c + 5));
        EXPECT_EQ(col.z, static_cast<TypeParam>(c + 9));
        EXPECT_EQ(col.w, static_cast<TypeParam>(c + 13));
    }
}

TYPED_TEST(Matrix4x4tTest, IndexOperatorSingleArgRead)
{
    using Mat4 = TestFixture::Mat4;
    // Storage is column-major: elements[i] is logical row (i % 4), column (i / 4).
    Mat4 m(
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16
    );
    for (int i = 0; i < 16; ++i)
    {
        int row = i % 4;
        int col = i / 4;
        EXPECT_EQ(m[i], static_cast<TypeParam>(row * 4 + col + 1));
    }
}

TYPED_TEST(Matrix4x4tTest, IndexOperatorSingleArgWrite)
{
    using Mat4 = TestFixture::Mat4;
    Mat4 m;
    m[0] = static_cast<TypeParam>(99);
    EXPECT_EQ(m[0], static_cast<TypeParam>(99));
    m[15] = static_cast<TypeParam>(-7);
    EXPECT_EQ(m[15], static_cast<TypeParam>(-7));
}

TYPED_TEST(Matrix4x4tTest, IndexOperatorTwoArgsRead)
{
    using Mat4 = TestFixture::Mat4;
    // Storage is column-major: elements[i] is logical row (i % 4), column (i / 4).
    Mat4 m(
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16
    );
    EXPECT_EQ(m[0], 1);
    EXPECT_EQ(m[6], 10);
    EXPECT_EQ(m[15], 16);
    EXPECT_EQ(m[3], 13);
    EXPECT_EQ(m[12], 4);
}

TYPED_TEST(Matrix4x4tTest, IndexOperatorTwoArgsWrite)
{
    using Mat4 = TestFixture::Mat4;
    Mat4 m;
    m[2, 1] = static_cast<TypeParam>(77); // row 2, column 1 -> flat index (column-major) = 1*4 + 2 = 6
    EXPECT_EQ(m[6], static_cast<TypeParam>(77));
    m[0, 3] = static_cast<TypeParam>(-5); // row 0, column 3 -> flat index (column-major) = 3*4 + 0 = 12
    EXPECT_EQ(m[12], static_cast<TypeParam>(-5));
}

// ─────────────────────────────────────────────
// Equality operators
// ─────────────────────────────────────────────

TYPED_TEST(Matrix4x4tTest, EqualityIdenticalMatrices)
{
    using Mat4 = TestFixture::Mat4;
    Mat4 a;
    Mat4 b;
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
}

TYPED_TEST(Matrix4x4tTest, InequalityDifferentMatrices)
{
    using Mat4 = TestFixture::Mat4;
    Mat4 a;
    Mat4 b;
    b[0] = static_cast<TypeParam>(99);
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
}

TYPED_TEST(Matrix4x4tTest, EqualitySymmetric)
{
    using Mat4 = TestFixture::Mat4;
    Mat4 a(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
    Mat4 b(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
    EXPECT_TRUE(a == b);
    EXPECT_TRUE(b == a);
}

// ─────────────────────────────────────────────
// Arithmetic operators
// ─────────────────────────────────────────────

TYPED_TEST(Matrix4x4tTest, Addition)
{
    using Mat4 = TestFixture::Mat4;
    Mat4 a = Mat4::Identity();
    Mat4 b = Mat4::Identity();
    Mat4 c = a + b;
    // identity + identity: diagonal = 2, off-diagonal = 0
    EXPECT_EQ(c.m00, 2); EXPECT_EQ(c.m11, 2);
    EXPECT_EQ(c.m22, 2); EXPECT_EQ(c.m33, 2);
    EXPECT_EQ(c.m01, 0);
}

TYPED_TEST(Matrix4x4tTest, AdditionAssignment)
{
    using Mat4 = TestFixture::Mat4;
    Mat4 a = Mat4::Identity();
    Mat4 b = Mat4::Identity();
    a += b;
    EXPECT_EQ(a.m00, 2); EXPECT_EQ(a.m11, 2);
    EXPECT_EQ(a.m01, 0);
}

TYPED_TEST(Matrix4x4tTest, Subtraction)
{
    using Mat4 = TestFixture::Mat4;
    Mat4 a = Mat4::Identity();
    Mat4 b = Mat4::Identity();
    Mat4 c = a - b;
    for (int i = 0; i < Mat4::ELEMENT_COUNT; ++i)
        EXPECT_EQ(c.elements[i], 0);
}

TYPED_TEST(Matrix4x4tTest, SubtractionAssignment)
{
    using Mat4 = TestFixture::Mat4;
    Mat4 a = Mat4::Identity();
    Mat4 b = Mat4::Identity();
    a -= b;
    for (int i = 0; i < Mat4::ELEMENT_COUNT; ++i)
        EXPECT_EQ(a.elements[i], 0);
}

TYPED_TEST(Matrix4x4tTest, ScalarMultiplication)
{
    using Mat4 = TestFixture::Mat4;
    Mat4 a = Mat4::Identity();
    Mat4 b = a * static_cast<TypeParam>(3);
    EXPECT_EQ(b.m00, 3); EXPECT_EQ(b.m11, 3);
    EXPECT_EQ(b.m22, 3); EXPECT_EQ(b.m33, 3);
    EXPECT_EQ(b.m01, 0);
}

TYPED_TEST(Matrix4x4tTest, ScalarMultiplicationAssignment)
{
    using Mat4 = TestFixture::Mat4;
    Mat4 a = Mat4::Identity();
    a *= static_cast<TypeParam>(2);
    EXPECT_EQ(a.m00, 2); EXPECT_EQ(a.m11, 2);
    EXPECT_EQ(a.m01, 0);
}

TYPED_TEST(Matrix4x4tTest, ScalarMultiplyByZero)
{
    using Mat4 = TestFixture::Mat4;
    Mat4 m = Mat4::Identity();
    Mat4 zero = m * static_cast<TypeParam>(0);
    for (int i = 0; i < Mat4::ELEMENT_COUNT; ++i)
        EXPECT_EQ(zero.elements[i], 0);
}

TYPED_TEST(Matrix4x4tTest, MatrixMultiplicationByIdentity)
{
    using Mat4 = TestFixture::Mat4;
    Mat4 m(
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16
    );
    Mat4 result = m * Mat4::Identity();
    EXPECT_TRUE(Mat4Equal<TypeParam>(result, m));
}

TYPED_TEST(Matrix4x4tTest, MatrixMultiplicationKnownResult)
{
    using Mat4 = TestFixture::Mat4;
    Mat4 a(
        1, 2, 0, 0,
        3, 4, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    );
    Mat4 b(
        5, 6, 0, 0,
        7, 8, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    );
    // [1 2] * [5 6] = [19 22]
    // [3 4]   [7 8]   [43 50]
    Mat4 c = a * b;
    EXPECT_NEAR(c.m00, 19, 1e-4);
    EXPECT_NEAR(c.m01, 22, 1e-4);
    EXPECT_NEAR(c.m10, 43, 1e-4);
    EXPECT_NEAR(c.m11, 50, 1e-4);
    EXPECT_NEAR(c.m22, 1, 1e-4);
    EXPECT_NEAR(c.m33, 1, 1e-4);
}

TYPED_TEST(Matrix4x4tTest, MatrixMultiplicationNotCommutative)
{
    using Mat4 = TestFixture::Mat4;
    using Vec3 = TestFixture::Vec3;
    Mat4 a = Mat4::Translation(Vec3(1, 0, 0));
    Mat4 b = Mat4::Scale(Vec3(2, 2, 2));
    Mat4 ab = a * b;
    Mat4 ba = b * a;
    EXPECT_FALSE(Mat4Equal<TypeParam>(ab, ba));
}

TYPED_TEST(Matrix4x4tTest, MatrixMultiplicationAssignment)
{
    using Mat4 = TestFixture::Mat4;
    Mat4 m(
        1, 2, 0, 0,
        3, 4, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    );
    Mat4 copy = m;
    m *= Mat4::Identity();
    EXPECT_TRUE(Mat4Equal<TypeParam>(m, copy));
}

// ─────────────────────────────────────────────
// Transpose
// ─────────────────────────────────────────────

TYPED_TEST(Matrix4x4tTest, TransposedReturnsCorrectMatrix)
{
    using Mat4 = TestFixture::Mat4;
    Mat4 m(
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16
    );
    Mat4 t = m.Transposed();

    // Off-diagonal elements are swapped
    EXPECT_EQ(t.m01, 5);
    EXPECT_EQ(t.m02, 9);
    EXPECT_EQ(t.m03, 13);
    EXPECT_EQ(t.m12, 10);
    // Diagonal unchanged
    EXPECT_EQ(t.m00, 1);
    EXPECT_EQ(t.m11, 6);
    EXPECT_EQ(t.m22, 11);
    EXPECT_EQ(t.m33, 16);
}

TYPED_TEST(Matrix4x4tTest, TransposedDoesNotModifyOriginal)
{
    using Mat4 = TestFixture::Mat4;
    Mat4 original(
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16
    );
    Mat4 copy = original;
    [[maybe_unused]] Mat4 t = original.Transposed();
    EXPECT_TRUE(Mat4Equal<TypeParam>(original, copy));
}

TYPED_TEST(Matrix4x4tTest, TransposeModifiesInPlace)
{
    using Mat4 = TestFixture::Mat4;
    Mat4 m(
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16
    );
    Mat4 expected = m.Transposed();
    m.Transpose();
    EXPECT_TRUE(Mat4Equal<TypeParam>(m, expected));
}

TYPED_TEST(Matrix4x4tTest, TransposeOfIdentityIsIdentity)
{
    using Mat4 = TestFixture::Mat4;
    Mat4 id = Mat4::Identity();
    EXPECT_TRUE(Mat4Equal<TypeParam>(id.Transposed(), Mat4::Identity()));
}

TYPED_TEST(Matrix4x4tTest, DoubleTransposeIsOriginal)
{
    using Mat4 = TestFixture::Mat4;
    Mat4 m(
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16
    );
    EXPECT_TRUE(Mat4Equal<TypeParam>(m.Transposed().Transposed(), m));
}

// ─────────────────────────────────────────────
// Determinant
// ─────────────────────────────────────────────

TYPED_TEST(Matrix4x4tTest, IdentityDeterminantIsOne)
{
    using Mat4 = TestFixture::Mat4;
    EXPECT_NEAR(Mat4::Identity().Determinant(), 1, 1e-4);
}

TYPED_TEST(Matrix4x4tTest, SingularMatrixDeterminantIsZero)
{
    using Mat4 = TestFixture::Mat4;
    Mat4 m(
        1, 2, 3, 4,
        1, 2, 3, 4,   // duplicate row → singular
        0, 0, 1, 0,
        0, 0, 0, 1
    );
    EXPECT_NEAR(m.Determinant(), 0, 1e-4);
}

TYPED_TEST(Matrix4x4tTest, UpperTriangularKnownValue)
{
    using Mat4 = TestFixture::Mat4;
    // det = 2 * 3 * 4 * 5 = 120
    Mat4 m(
        2, 1, 1, 1,
        0, 3, 1, 1,
        0, 0, 4, 1,
        0, 0, 0, 5
    );
    EXPECT_NEAR(m.Determinant(), 120, 1e-4);
}

TYPED_TEST(Matrix4x4tTest, ScaleMatrixDeterminant)
{
    using Mat4 = TestFixture::Mat4;
    using Vec3 = TestFixture::Vec3;
    // det(diag(a,b,c,1)) = a*b*c
    Mat4 m = Mat4::Scale(Vec3(2, 3, 4));
    EXPECT_NEAR(m.Determinant(), 24, 1e-4);
}

TYPED_TEST(Matrix4x4tTest, DeterminantOfProductEqualsProductOfDeterminants)
{
    using Mat4 = TestFixture::Mat4;
    using Vec3 = TestFixture::Vec3;
    Mat4 a = Mat4::Scale(Vec3(1, 2, 3));
    Mat4 b = Mat4::Scale(Vec3(4, 5, 6));
    TypeParam detAB = (a * b).Determinant();
    TypeParam detAdotdetB = a.Determinant() * b.Determinant();
    EXPECT_NEAR(detAB, detAdotdetB, 1e-4);
}

// ─────────────────────────────────────────────
// Inverse
// ─────────────────────────────────────────────

TYPED_TEST(Matrix4x4tTest, InversedOfIdentityIsIdentity)
{
    using Mat4 = TestFixture::Mat4;
    EXPECT_TRUE(Mat4Equal<TypeParam>(Mat4::Identity().Inversed(), Mat4::Identity()));
}

TYPED_TEST(Matrix4x4tTest, InversedMultipliedGivesIdentity)
{
    using Mat4 = TestFixture::Mat4;
    using Vec3 = TestFixture::Vec3;
    Mat4 m = Mat4::Translation(Vec3(1, 2, 3));
    Mat4 product = m * m.Inversed();
    EXPECT_TRUE(Mat4Equal<TypeParam>(product, Mat4::Identity()));
}

TYPED_TEST(Matrix4x4tTest, InversedDoesNotModifyOriginal)
{
    using Mat4 = TestFixture::Mat4;
    using Vec3 = TestFixture::Vec3;
    Mat4 m = Mat4::Translation(Vec3(1, 2, 3));
    Mat4 copy = m;
    [[maybe_unused]] Mat4 inv = m.Inversed();
    EXPECT_TRUE(Mat4Equal<TypeParam>(m, copy));
}

TYPED_TEST(Matrix4x4tTest, InverseModifiesInPlace)
{
    using Mat4 = TestFixture::Mat4;
    using Vec3 = TestFixture::Vec3;
    Mat4 m = Mat4::Translation(Vec3(5, -3, 7));
    Mat4 original = m;
    m.Inverse();
    Mat4 product = original * m;
    EXPECT_TRUE(Mat4Equal<TypeParam>(product, Mat4::Identity()));
}

TYPED_TEST(Matrix4x4tTest, DoubleInverseIsOriginal)
{
    using Mat4 = TestFixture::Mat4;
    using Vec3 = TestFixture::Vec3;
    using Quat = TestFixture::Quat;
    using RadianT = TestFixture::RadianT;
    Mat4 m = Mat4::TRS(
        Vec3(1, 2, 3),
        Quat::FromAngleAxis(RadianT(static_cast<TypeParam>(0.5)), Vec3::Up()),
        Vec3(1, 1, 1)
    );
    EXPECT_TRUE(Mat4Equal<TypeParam>(m.Inversed().Inversed(), m, static_cast<TypeParam>(1e-3)));
}

// ─────────────────────────────────────────────
// Decomposition: Translation / Rotation / Scale
// ─────────────────────────────────────────────

TYPED_TEST(Matrix4x4tTest, GetTranslationFromIdentity)
{
    using Mat4 = TestFixture::Mat4;
    Mat4 id = Mat4::Identity();
    auto t = id.GetTranslation();
    EXPECT_NEAR(t.x, 0, 1e-4);
    EXPECT_NEAR(t.y, 0, 1e-4);
    EXPECT_NEAR(t.z, 0, 1e-4);
}

TYPED_TEST(Matrix4x4tTest, GetTranslation)
{
    using Mat4 = TestFixture::Mat4;
    using Vec3 = TestFixture::Vec3;
    Vec3 expected(3, -1, 7);
    Mat4 m = Mat4::Translation(expected);
    EXPECT_TRUE(Vec3Equal<TypeParam>(m.GetTranslation(), expected));
}

TYPED_TEST(Matrix4x4tTest, GetScaleUniform)
{
    using Mat4 = TestFixture::Mat4;
    using Vec3 = TestFixture::Vec3;
    Vec3 s(4, 4, 4);
    Mat4 m = Mat4::Scale(s);
    Vec3 extracted = m.GetScale();
    EXPECT_NEAR(extracted.x, s.x, 1e-4);
    EXPECT_NEAR(extracted.y, s.y, 1e-4);
    EXPECT_NEAR(extracted.z, s.z, 1e-4);
}

TYPED_TEST(Matrix4x4tTest, GetScaleNonUniform)
{
    using Mat4 = TestFixture::Mat4;
    using Vec3 = TestFixture::Vec3;
    Vec3 s(2, 3, 4);
    Mat4 m = Mat4::Scale(s);
    Vec3 extracted = m.GetScale();
    EXPECT_NEAR(extracted.x, s.x, 1e-4);
    EXPECT_NEAR(extracted.y, s.y, 1e-4);
    EXPECT_NEAR(extracted.z, s.z, 1e-4);
}

TYPED_TEST(Matrix4x4tTest, GetScaleFromIdentity)
{
    using Mat4 = TestFixture::Mat4;
    auto s = Mat4::Identity().GetScale();
    EXPECT_NEAR(s.x, 1, 1e-4);
    EXPECT_NEAR(s.y, 1, 1e-4);
    EXPECT_NEAR(s.z, 1, 1e-4);
}

TYPED_TEST(Matrix4x4tTest, GetRotationIdentity)
{
    using Mat4 = TestFixture::Mat4;
    using Quat = TestFixture::Quat;
    Quat q = Mat4::Identity().GetRotation();
    Quat identity = Quat::Identity();

    bool sameOrNegated =
        (std::fabs(q.x - identity.x) < 1e-4 && std::fabs(q.y - identity.y) < 1e-4 &&
            std::fabs(q.z - identity.z) < 1e-4 && std::fabs(q.w - identity.w) < 1e-4) ||
        (std::fabs(q.x + identity.x) < 1e-4 && std::fabs(q.y + identity.y) < 1e-4 &&
            std::fabs(q.z + identity.z) < 1e-4 && std::fabs(q.w + identity.w) < 1e-4);

    EXPECT_TRUE(sameOrNegated);
}

TYPED_TEST(Matrix4x4tTest, GetRotation90DegAroundY)
{
    using Mat4 = TestFixture::Mat4;
    using Vec3 = TestFixture::Vec3;
    using Quat = TestFixture::Quat;
    Quat q = Quat::FromAngleAxis(TestFixture::RadianT::PI_DIV_2(), Vec3::Up());
    Mat4 m = Mat4::Rotation(q);
    Quat extracted = m.GetRotation();
    bool sameOrNegated =
        (std::fabs(extracted.x - q.x) < 1e-4 && std::fabs(extracted.y - q.y) < 1e-4 &&
            std::fabs(extracted.z - q.z) < 1e-4 && std::fabs(extracted.w - q.w) < 1e-4) ||
        (std::fabs(extracted.x + q.x) < 1e-4 && std::fabs(extracted.y + q.y) < 1e-4 &&
            std::fabs(extracted.z + q.z) < 1e-4 && std::fabs(extracted.w + q.w) < 1e-4);
    EXPECT_TRUE(sameOrNegated);
}

// ─────────────────────────────────────────────
// Static factory methods
// ─────────────────────────────────────────────

TYPED_TEST(Matrix4x4tTest, CreateTranslationStoresTranslation)
{
    using Mat4 = TestFixture::Mat4;
    using Vec3 = TestFixture::Vec3;
    Vec3 t(5, -3, 2);
    Mat4 m = Mat4::Translation(t);
    EXPECT_TRUE(Vec3Equal<TypeParam>(m.GetTranslation(), t));
}

TYPED_TEST(Matrix4x4tTest, CreateTranslationUpperLeft3x3IsIdentity)
{
    using Mat4 = TestFixture::Mat4;
    using Vec3 = TestFixture::Vec3;
    Mat4 m = Mat4::Translation(Vec3(1, 2, 3));
    EXPECT_NEAR(m.m00, 1, 1e-4); EXPECT_NEAR(m.m11, 1, 1e-4);
    EXPECT_NEAR(m.m22, 1, 1e-4); EXPECT_NEAR(m.m01, 0, 1e-4);
}

TYPED_TEST(Matrix4x4tTest, CreateTranslationZeroEqualsIdentity)
{
    using Mat4 = TestFixture::Mat4;
    using Vec3 = TestFixture::Vec3;
    Mat4 m = Mat4::Translation(Vec3(0, 0, 0));
    EXPECT_TRUE(Mat4Equal<TypeParam>(m, Mat4::Identity()));
}

TYPED_TEST(Matrix4x4tTest, CreateScaleDiagonal)
{
    using Mat4 = TestFixture::Mat4;
    using Vec3 = TestFixture::Vec3;
    Vec3 s(2, 3, 4);
    Mat4 m = Mat4::Scale(s);
    EXPECT_NEAR(m.m00, s.x, 1e-4);
    EXPECT_NEAR(m.m11, s.y, 1e-4);
    EXPECT_NEAR(m.m22, s.z, 1e-4);
    EXPECT_NEAR(m.m33, 1, 1e-4);
    EXPECT_NEAR(m.m01, 0, 1e-4);
}

TYPED_TEST(Matrix4x4tTest, CreateScaleOneEqualsIdentity)
{
    using Mat4 = TestFixture::Mat4;
    using Vec3 = TestFixture::Vec3;
    Mat4 m = Mat4::Scale(Vec3(1, 1, 1));
    EXPECT_TRUE(Mat4Equal<TypeParam>(m, Mat4::Identity()));
}

TYPED_TEST(Matrix4x4tTest, RotationIdentityQuaternionGivesIdentityMatrix)
{
    using Mat4 = TestFixture::Mat4;
    using Quat = TestFixture::Quat;
    Quat q = Quat::Identity();
    Mat4 m = Mat4::Rotation(q);
    EXPECT_TRUE(Mat4Equal<TypeParam>(m, Mat4::Identity()));
}

TYPED_TEST(Matrix4x4tTest, Rotation180DegAroundXFlipsYZ)
{
    using Mat4 = TestFixture::Mat4;
    using Vec3 = TestFixture::Vec3;
    using Vec4 = TestFixture::Vec4;
    using Quat = TestFixture::Quat;
    using RadianT = TestFixture::RadianT;
    Quat q = Quat::FromAngleAxis(RadianT(static_cast<TypeParam>(Mathf::PI)), Vec3::Right());
    Mat4 m = Mat4::Rotation(q);
    Vec3 v = (m * Vec4(0, 1, 0, 0)).template xyz<TypeParam>();
    EXPECT_NEAR(v.x, 0, 1e-4);
    EXPECT_NEAR(v.y, -1, 1e-4);
}

TYPED_TEST(Matrix4x4tTest, TRSTranslationRoundTrip)
{
    using Mat4 = TestFixture::Mat4;
    using Vec3 = TestFixture::Vec3;
    using Quat = TestFixture::Quat;
    Vec3 t(1, 2, 3);
    Mat4 m = Mat4::TRS(t, Quat(), Vec3(1, 1, 1));
    EXPECT_TRUE(Vec3Equal<TypeParam>(m.GetTranslation(), t));
}

TYPED_TEST(Matrix4x4tTest, TRSScaleRoundTrip)
{
    using Mat4 = TestFixture::Mat4;
    using Vec3 = TestFixture::Vec3;
    using Quat = TestFixture::Quat;
    Vec3 s(2, 3, 4);
    Mat4 m = Mat4::TRS(Vec3(0, 0, 0), Quat(), s);
    Vec3 extractedScale = m.GetScale();
    EXPECT_NEAR(extractedScale.x, s.x, 1e-4);
    EXPECT_NEAR(extractedScale.y, s.y, 1e-4);
    EXPECT_NEAR(extractedScale.z, s.z, 1e-4);
}

TYPED_TEST(Matrix4x4tTest, TRSIdentityEqualsIdentity)
{
    using Mat4 = TestFixture::Mat4;
    using Vec3 = TestFixture::Vec3;
    using Quat = TestFixture::Quat;
    Mat4 m = Mat4::TRS(
        Vec3(0, 0, 0),
        Quat(),
        Vec3(1, 1, 1)
    );
    EXPECT_TRUE(Mat4Equal<TypeParam>(m, Mat4::Identity()));
}

// ─────────────────────────────────────────────
// Projection matrices
// ─────────────────────────────────────────────

TYPED_TEST(Matrix4x4tTest, PerspectiveDiagonalElementsAreFiniteAndPositive)
{
    using Mat4 = TestFixture::Mat4;
    using RadianT = TestFixture::RadianT;
    Mat4 m = Mat4::PerspectiveProjection(
        RadianT(static_cast<TypeParam>(Mathf::PI) / static_cast<TypeParam>(2)),
        static_cast<TypeParam>(16) / static_cast<TypeParam>(9),
        static_cast<TypeParam>(0.1),
        static_cast<TypeParam>(1000)
    );
    EXPECT_TRUE(std::isfinite(m.m00)); EXPECT_GT(m.m00, 0);
    EXPECT_TRUE(std::isfinite(m.m11)); EXPECT_GT(m.m11, 0);
    EXPECT_TRUE(std::isfinite(m.m22));
    EXPECT_TRUE(std::isfinite(m.m23));
}

TYPED_TEST(Matrix4x4tTest, PerspectiveNarrowFovHasLargerM11)
{
    using Mat4 = TestFixture::Mat4;
    using RadianT = TestFixture::RadianT;
    Mat4 wide = Mat4::PerspectiveProjection(RadianT(static_cast<TypeParam>(Mathf::PI) / static_cast<TypeParam>(2)), static_cast<TypeParam>(1), static_cast<TypeParam>(0.1), static_cast<TypeParam>(100));
    Mat4 narrow = Mat4::PerspectiveProjection(RadianT(static_cast<TypeParam>(Mathf::PI) / static_cast<TypeParam>(4)), static_cast<TypeParam>(1), static_cast<TypeParam>(0.1), static_cast<TypeParam>(100));
    EXPECT_GT(narrow.m11, wide.m11);
}

TYPED_TEST(Matrix4x4tTest, OrthographicSymmetricScaleIsOne)
{
    using Mat4 = TestFixture::Mat4;
    // Symmetric unit-cube mapping: l=-1,r=1,t=1,b=-1,n=0,f=1
    Mat4 m = Mat4::OrthographicProjection(
        static_cast<TypeParam>(-1), static_cast<TypeParam>(1), static_cast<TypeParam>(1),
        static_cast<TypeParam>(-1), static_cast<TypeParam>(0), static_cast<TypeParam>(1)
    );
    EXPECT_NEAR(std::fabs(m.m00), 1, 1e-4);
    EXPECT_NEAR(std::fabs(m.m11), 1, 1e-4);
    EXPECT_TRUE(std::isfinite(m.m22));
}

TYPED_TEST(Matrix4x4tTest, OrthographicElementsAreFinite)
{
    using Mat4 = TestFixture::Mat4;
    Mat4 m = Mat4::OrthographicProjection(
        static_cast<TypeParam>(0), static_cast<TypeParam>(800), static_cast<TypeParam>(600),
        static_cast<TypeParam>(0), static_cast<TypeParam>(-1), static_cast<TypeParam>(1)
    );
    for (int i = 0; i < Mat4::ELEMENT_COUNT; ++i)
        EXPECT_TRUE(std::isfinite(m.elements[i]));
}

// ─────────────────────────────────────────────
// LookAt
// ─────────────────────────────────────────────

TYPED_TEST(Matrix4x4tTest, LookAtUpperLeft3x3IsOrthonormal)
{
    using Mat4 = TestFixture::Mat4;
    using Vec3 = TestFixture::Vec3;
    Mat4 m = Mat4::LookAt(
        Vec3(0, 0, 5),
        Vec3(0, 0, 0),
        Vec3::Up()
    );
    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            TypeParam dot =
                m.GetRow(row).x * m.GetRow(col).x +
                m.GetRow(row).y * m.GetRow(col).y +
                m.GetRow(row).z * m.GetRow(col).z;
            TypeParam expected = (row == col) ? static_cast<TypeParam>(1) : static_cast<TypeParam>(0);
            EXPECT_NEAR(dot, expected, 1e-4);
        }
    }
}

TYPED_TEST(Matrix4x4tTest, LookAtDefaultUpParameterDoesNotThrow)
{
    using Mat4 = TestFixture::Mat4;
    using Vec3 = TestFixture::Vec3;
    EXPECT_NO_THROW({
        Mat4 m = Mat4::LookAt(
            Vec3(1, 1, 1),
            Vec3(0, 0, 0)
        );
        (void)m;
        });
}

TYPED_TEST(Matrix4x4tTest, LookAtCustomUpVector)
{
    using Mat4 = TestFixture::Mat4;
    using Vec3 = TestFixture::Vec3;
    EXPECT_NO_THROW({
        Mat4 m = Mat4::LookAt(
            Vec3(0, 5, 0),
            Vec3(0, 0, 0),
            Vec3::Forward()
        );
        (void)m;
        });
}

// ─────────────────────────────────────────────
// Robustness
// ─────────────────────────────────────────────

TYPED_TEST(Matrix4x4tTest, UniformScaleThenInverse)
{
    using Mat4 = TestFixture::Mat4;
    using Vec3 = TestFixture::Vec3;
    Mat4 s = Mat4::Scale(Vec3(2, 2, 2));
    EXPECT_TRUE(Mat4Equal<TypeParam>(s * s.Inversed(), Mat4::Identity()));
}

TYPED_TEST(Matrix4x4tTest, ChainedTRSAndInverse)
{
    using Mat4 = TestFixture::Mat4;
    using Vec3 = TestFixture::Vec3;
    using Quat = TestFixture::Quat;
    using RadianT = TestFixture::RadianT;
    Mat4 trs = Mat4::TRS(
        Vec3(3, -1, 2),
        Quat::FromAngleAxis(RadianT(static_cast<TypeParam>(0.785398)), Vec3(0, 1, 0)),
        Vec3(1, 1, 1)
    );
    EXPECT_TRUE(Mat4Equal<TypeParam>(trs * trs.Inversed(), Mat4::Identity(), static_cast<TypeParam>(1e-3)));
}

TYPED_TEST(Matrix4x4tTest, StaticIdentityColumnsMatchNamedColumns)
{
    using Mat4 = TestFixture::Mat4;
    EXPECT_EQ(Mat4::Identity().GetColumn(0), Mat4::IdentityColumn0());
    EXPECT_EQ(Mat4::Identity().GetColumn(1), Mat4::IdentityColumn1());
    EXPECT_EQ(Mat4::Identity().GetColumn(2), Mat4::IdentityColumn2());
    EXPECT_EQ(Mat4::Identity().GetColumn(3), Mat4::IdentityColumn3());
}

TYPED_TEST(Matrix4x4tTest, NegativeScalePreservesAbsoluteGetScale)
{
    using Mat4 = TestFixture::Mat4;
    using Vec3 = TestFixture::Vec3;
    Vec3 s(-1, 2, -3);
    Mat4 m = Mat4::Scale(s);
    Vec3 extracted = m.GetScale();
    // GetScale returns magnitudes of basis vectors
    EXPECT_NEAR(extracted.x, std::fabs(s.x), 1e-4);
    EXPECT_NEAR(extracted.y, std::fabs(s.y), 1e-4);
    EXPECT_NEAR(extracted.z, std::fabs(s.z), 1e-4);
}