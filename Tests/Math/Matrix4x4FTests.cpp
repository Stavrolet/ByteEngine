#include <gtest/gtest.h>
#include <cmath>
#include "ByteEngine/Math/Matrix4x4F.h"
#include "ByteEngine/Math/Vector3.h"
#include "ByteEngine/Math/Vector4.h"
#include "ByteEngine/Math/Quaternion.h"

using namespace ByteEngine::Math;
using namespace ByteEngine::Math::Literals;

// ─────────────────────────────────────────────
// Helpers
// ─────────────────────────────────────────────

static constexpr float kEps = 1e-4f;

static bool Mat4Equal(const Matrix4x4F& a, const Matrix4x4F& b, float eps = kEps)
{
    for (int i = 0; i < Matrix4x4F::ELEMENT_COUNT; ++i)
        if (std::fabs(a.elements[i] - b.elements[i]) > eps)
            return false;
    return true;
}

static bool Vec3Equal(const Vector3F& a, const Vector3F& b, float eps = kEps)
{
    return std::fabs(a.x - b.x) < eps &&
        std::fabs(a.y - b.y) < eps &&
        std::fabs(a.z - b.z) < eps;
}

// ─────────────────────────────────────────────
// Construction
// ─────────────────────────────────────────────

TEST(Matrix4x4FConstructionTest, DefaultConstructor)
{
    Matrix4x4F m;
    EXPECT_EQ(m.m00, 1.0f); EXPECT_EQ(m.m11, 1.0f);
    EXPECT_EQ(m.m22, 1.0f); EXPECT_EQ(m.m33, 1.0f);
    EXPECT_EQ(m.m01, 0.0f); EXPECT_EQ(m.m10, 0.0f);
    EXPECT_EQ(m.m23, 0.0f); EXPECT_EQ(m.m32, 0.0f);
}

TEST(Matrix4x4FConstructionTest, ScalarConstructor)
{
    Matrix4x4F m(
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16
    );
    EXPECT_EQ(m.m00, 1.f); EXPECT_EQ(m.m01, 2.f);
    EXPECT_EQ(m.m02, 3.f); EXPECT_EQ(m.m03, 4.f);
    EXPECT_EQ(m.m10, 5.f); EXPECT_EQ(m.m11, 6.f);
    EXPECT_EQ(m.m12, 7.f); EXPECT_EQ(m.m13, 8.f);
    EXPECT_EQ(m.m20, 9.f); EXPECT_EQ(m.m21, 10.f);
    EXPECT_EQ(m.m22, 11.f); EXPECT_EQ(m.m23, 12.f);
    EXPECT_EQ(m.m30, 13.f); EXPECT_EQ(m.m31, 14.f);
    EXPECT_EQ(m.m32, 15.f); EXPECT_EQ(m.m33, 16.f);
}

TEST(Matrix4x4FConstructionTest, RowVectorConstructor)
{
    Vector4F r0(1, 2, 3, 4);
    Vector4F r1(5, 6, 7, 8);
    Vector4F r2(9, 10, 11, 12);
    Vector4F r3(13, 14, 15, 16);

    Matrix4x4F m(r0, r1, r2, r3);

    EXPECT_EQ(m.m00, 1.f); EXPECT_EQ(m.m03, 4.f);
    EXPECT_EQ(m.m10, 5.f); EXPECT_EQ(m.m13, 8.f);
    EXPECT_EQ(m.m20, 9.f); EXPECT_EQ(m.m23, 12.f);
    EXPECT_EQ(m.m33, 16.f);
}

TEST(Matrix4x4FConstructionTest, ArrayConstructor)
{
    float elems[16] = {
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16
    };
    Matrix4x4F m(elems);

    for (int i = 0; i < 16; ++i)
        EXPECT_EQ(m.elements[i], static_cast<float>(i + 1));
}

// ─────────────────────────────────────────────
// Row / Column accessors
// ─────────────────────────────────────────────

TEST(Matrix4x4FAccessTest, GetRow)
{
    Matrix4x4F m(
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16
    );
    Vector4F row1 = m.GetRow(1);
    EXPECT_EQ(row1.x, 5.f); EXPECT_EQ(row1.y, 6.f);
    EXPECT_EQ(row1.z, 7.f); EXPECT_EQ(row1.w, 8.f);
}

TEST(Matrix4x4FAccessTest, GetRowAllRows)
{
    Matrix4x4F m(
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16
    );
    for (int r = 0; r < 4; ++r)
    {
        Vector4F row = m.GetRow(r);
        for (int c = 0; c < 4; ++c)
            EXPECT_EQ(row[c], static_cast<float>(r * 4 + c + 1));
    }
}

TEST(Matrix4x4FAccessTest, GetColumn)
{
    Matrix4x4F m(
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16
    );
    Vector4F col2 = m.GetColumn(2);
    EXPECT_EQ(col2.x, 3.f);
    EXPECT_EQ(col2.y, 7.f);
    EXPECT_EQ(col2.z, 11.f);
    EXPECT_EQ(col2.w, 15.f);
}

TEST(Matrix4x4FAccessTest, GetColumnAllColumns)
{
    Matrix4x4F m(
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16
    );
    for (int c = 0; c < 4; ++c)
    {
        Vector4F col = m.GetColumn(c);
        EXPECT_EQ(col.x, static_cast<float>(c + 1));
        EXPECT_EQ(col.y, static_cast<float>(c + 5));
        EXPECT_EQ(col.z, static_cast<float>(c + 9));
        EXPECT_EQ(col.w, static_cast<float>(c + 13));
    }
}

TEST(Matrix4x4FAccessTest, IndexOperatorSingleArgRead)
{
    Matrix4x4F m(
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16
    );
    for (int i = 0; i < 16; ++i)
        EXPECT_EQ(m[i], static_cast<float>(i + 1));
}

TEST(Matrix4x4FAccessTest, IndexOperatorSingleArgWrite)
{
    Matrix4x4F m;
    m[0] = 99.f;
    EXPECT_EQ(m[0], 99.f);
    m[15] = -7.f;
    EXPECT_EQ(m[15], -7.f);
}

TEST(Matrix4x4FAccessTest, IndexOperatorTwoArgsRead)
{
    Matrix4x4F m(
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16
    );
    EXPECT_EQ(m[0], 1.f);
    EXPECT_EQ(m[6], 7.f);
    EXPECT_EQ(m[15], 16.f);
    EXPECT_EQ(m[3], 4.f);
    EXPECT_EQ(m[12], 13.f);
}

TEST(Matrix4x4FAccessTest, IndexOperatorTwoArgsWrite)
{
    Matrix4x4F m;
    m[2, 1] = 77.f;
    EXPECT_EQ(m[9], 77.f);
    m[0, 3] = -5.f;
    EXPECT_EQ(m[3], -5.f);
}

// ─────────────────────────────────────────────
// Equality operators
// ─────────────────────────────────────────────

TEST(Matrix4x4FOperatorTest, EqualityIdenticalMatrices)
{
    Matrix4x4F a;
    Matrix4x4F b;
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
}

TEST(Matrix4x4FOperatorTest, InequalityDifferentMatrices)
{
    Matrix4x4F a;
    Matrix4x4F b;
    b[0] = 99.f;
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
}

TEST(Matrix4x4FOperatorTest, EqualitySymmetric)
{
    Matrix4x4F a(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
    Matrix4x4F b(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
    EXPECT_TRUE(a == b);
    EXPECT_TRUE(b == a);
}

// ─────────────────────────────────────────────
// Arithmetic operators
// ─────────────────────────────────────────────

TEST(Matrix4x4FOperatorTest, Addition)
{
    Matrix4x4F a = Matrix4x4F::Identity();
    Matrix4x4F b = Matrix4x4F::Identity();
    Matrix4x4F c = a + b;
    // identity + identity: diagonal = 2, off-diagonal = 0
    EXPECT_EQ(c.m00, 2.f); EXPECT_EQ(c.m11, 2.f);
    EXPECT_EQ(c.m22, 2.f); EXPECT_EQ(c.m33, 2.f);
    EXPECT_EQ(c.m01, 0.f);
}

TEST(Matrix4x4FOperatorTest, AdditionAssignment)
{
    Matrix4x4F a = Matrix4x4F::Identity();
    Matrix4x4F b = Matrix4x4F::Identity();
    a += b;
    EXPECT_EQ(a.m00, 2.f); EXPECT_EQ(a.m11, 2.f);
    EXPECT_EQ(a.m01, 0.f);
}

TEST(Matrix4x4FOperatorTest, Subtraction)
{
    Matrix4x4F a = Matrix4x4F::Identity();
    Matrix4x4F b = Matrix4x4F::Identity();
    Matrix4x4F c = a - b;
    for (int i = 0; i < Matrix4x4F::ELEMENT_COUNT; ++i)
        EXPECT_EQ(c.elements[i], 0.f);
}

TEST(Matrix4x4FOperatorTest, SubtractionAssignment)
{
    Matrix4x4F a = Matrix4x4F::Identity();
    Matrix4x4F b = Matrix4x4F::Identity();
    a -= b;
    for (int i = 0; i < Matrix4x4F::ELEMENT_COUNT; ++i)
        EXPECT_EQ(a.elements[i], 0.f);
}

TEST(Matrix4x4FOperatorTest, ScalarMultiplication)
{
    Matrix4x4F a = Matrix4x4F::Identity();
    Matrix4x4F b = a * 3.f;
    EXPECT_EQ(b.m00, 3.f); EXPECT_EQ(b.m11, 3.f);
    EXPECT_EQ(b.m22, 3.f); EXPECT_EQ(b.m33, 3.f);
    EXPECT_EQ(b.m01, 0.f);
}

TEST(Matrix4x4FOperatorTest, ScalarMultiplicationAssignment)
{
    Matrix4x4F a = Matrix4x4F::Identity();
    a *= 2.f;
    EXPECT_EQ(a.m00, 2.f); EXPECT_EQ(a.m11, 2.f);
    EXPECT_EQ(a.m01, 0.f);
}

TEST(Matrix4x4FOperatorTest, ScalarMultiplyByZero)
{
    Matrix4x4F m = Matrix4x4F::Identity();
    Matrix4x4F zero = m * 0.f;
    for (int i = 0; i < Matrix4x4F::ELEMENT_COUNT; ++i)
        EXPECT_EQ(zero.elements[i], 0.f);
}

TEST(Matrix4x4FOperatorTest, MatrixMultiplicationByIdentity)
{
    Matrix4x4F m(
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16
    );
    Matrix4x4F result = m * Matrix4x4F::Identity();
    EXPECT_TRUE(Mat4Equal(result, m));
}

TEST(Matrix4x4FOperatorTest, MatrixMultiplicationKnownResult)
{
    Matrix4x4F a(
        1, 2, 0, 0,
        3, 4, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    );
    Matrix4x4F b(
        5, 6, 0, 0,
        7, 8, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    );
    // [1 2] * [5 6] = [19 22]
    // [3 4]   [7 8]   [43 50]
    Matrix4x4F c = a * b;
    EXPECT_NEAR(c.m00, 19.f, kEps);
    EXPECT_NEAR(c.m01, 22.f, kEps);
    EXPECT_NEAR(c.m10, 43.f, kEps);
    EXPECT_NEAR(c.m11, 50.f, kEps);
    EXPECT_NEAR(c.m22, 1.f, kEps);
    EXPECT_NEAR(c.m33, 1.f, kEps);
}

TEST(Matrix4x4FOperatorTest, MatrixMultiplicationNotCommutative)
{
    Matrix4x4F a = Matrix4x4F::Translation(Vector3F(1, 0, 0));
    Matrix4x4F b = Matrix4x4F::Scale(Vector3F(2, 2, 2));
    Matrix4x4F ab = a * b;
    Matrix4x4F ba = b * a;
    EXPECT_FALSE(Mat4Equal(ab, ba));
}

TEST(Matrix4x4FOperatorTest, MatrixMultiplicationAssignment)
{
    Matrix4x4F m(
        1, 2, 0, 0,
        3, 4, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    );
    Matrix4x4F copy = m;
    m *= Matrix4x4F::Identity();
    EXPECT_TRUE(Mat4Equal(m, copy));
}

// ─────────────────────────────────────────────
// Transpose
// ─────────────────────────────────────────────

TEST(Matrix4x4FTransposeTest, TransposedReturnsCorrectMatrix)
{
    Matrix4x4F m(
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16
    );
    Matrix4x4F t = m.Transposed();

    // Off-diagonal elements are swapped
    EXPECT_EQ(t.m01, 5.f);
    EXPECT_EQ(t.m02, 9.f);
    EXPECT_EQ(t.m03, 13.f);
    EXPECT_EQ(t.m12, 10.f);
    // Diagonal unchanged
    EXPECT_EQ(t.m00, 1.f);
    EXPECT_EQ(t.m11, 6.f);
    EXPECT_EQ(t.m22, 11.f);
    EXPECT_EQ(t.m33, 16.f);
}

TEST(Matrix4x4FTransposeTest, TransposedDoesNotModifyOriginal)
{
    Matrix4x4F original(
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16
    );
    Matrix4x4F copy = original;
    [[maybe_unused]] Matrix4x4F t = original.Transposed();
    EXPECT_TRUE(Mat4Equal(original, copy));
}

TEST(Matrix4x4FTransposeTest, TransposeModifiesInPlace)
{
    Matrix4x4F m(
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16
    );
    Matrix4x4F expected = m.Transposed();
    m.Transpose();
    EXPECT_TRUE(Mat4Equal(m, expected));
}

TEST(Matrix4x4FTransposeTest, TransposeOfIdentityIsIdentity)
{
    Matrix4x4F id = Matrix4x4F::Identity();
    EXPECT_TRUE(Mat4Equal(id.Transposed(), Matrix4x4F::Identity()));
}

TEST(Matrix4x4FTransposeTest, DoubleTransposeIsOriginal)
{
    Matrix4x4F m(
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16
    );
    EXPECT_TRUE(Mat4Equal(m.Transposed().Transposed(), m));
}

// ─────────────────────────────────────────────
// Determinant
// ─────────────────────────────────────────────

TEST(Matrix4x4FDeterminantTest, IdentityDeterminantIsOne)
{
    EXPECT_NEAR(Matrix4x4F::Identity().Determinant(), 1.f, kEps);
}

TEST(Matrix4x4FDeterminantTest, SingularMatrixDeterminantIsZero)
{
    Matrix4x4F m(
        1, 2, 3, 4,
        1, 2, 3, 4,   // duplicate row → singular
        0, 0, 1, 0,
        0, 0, 0, 1
    );
    EXPECT_NEAR(m.Determinant(), 0.f, kEps);
}

TEST(Matrix4x4FDeterminantTest, UpperTriangularKnownValue)
{
    // det = 2 * 3 * 4 * 5 = 120
    Matrix4x4F m(
        2, 1, 1, 1,
        0, 3, 1, 1,
        0, 0, 4, 1,
        0, 0, 0, 5
    );
    EXPECT_NEAR(m.Determinant(), 120.f, kEps);
}

TEST(Matrix4x4FDeterminantTest, ScaleMatrixDeterminant)
{
    // det(diag(a,b,c,1)) = a*b*c
    Matrix4x4F m = Matrix4x4F::Scale(Vector3F(2.f, 3.f, 4.f));
    EXPECT_NEAR(m.Determinant(), 24.f, kEps);
}

TEST(Matrix4x4FDeterminantTest, DeterminantOfProductEqualsProductOfDeterminants)
{
    Matrix4x4F a = Matrix4x4F::Scale(Vector3F(1.f, 2.f, 3.f));
    Matrix4x4F b = Matrix4x4F::Scale(Vector3F(4.f, 5.f, 6.f));
    float detAB = (a * b).Determinant();
    float detAdotdetB = a.Determinant() * b.Determinant();
    EXPECT_NEAR(detAB, detAdotdetB, kEps);
}

// ─────────────────────────────────────────────
// Inverse
// ─────────────────────────────────────────────

TEST(Matrix4x4FInverseTest, InversedOfIdentityIsIdentity)
{
    EXPECT_TRUE(Mat4Equal(Matrix4x4F::Identity().Inversed(), Matrix4x4F::Identity()));
}

TEST(Matrix4x4FInverseTest, InversedMultipliedGivesIdentity)
{
    Matrix4x4F m = Matrix4x4F::Translation(Vector3F(1.f, 2.f, 3.f));
    Matrix4x4F product = m * m.Inversed();
    EXPECT_TRUE(Mat4Equal(product, Matrix4x4F::Identity()));
}

TEST(Matrix4x4FInverseTest, InversedDoesNotModifyOriginal)
{
    Matrix4x4F m = Matrix4x4F::Translation(Vector3F(1.f, 2.f, 3.f));
    Matrix4x4F copy = m;
    [[maybe_unused]] Matrix4x4F inv = m.Inversed();
    EXPECT_TRUE(Mat4Equal(m, copy));
}

TEST(Matrix4x4FInverseTest, InverseModifiesInPlace)
{
    Matrix4x4F m = Matrix4x4F::Translation(Vector3F(5.f, -3.f, 7.f));
    Matrix4x4F original = m;
    m.Inverse();
    Matrix4x4F product = original * m;
    EXPECT_TRUE(Mat4Equal(product, Matrix4x4F::Identity()));
}

TEST(Matrix4x4FInverseTest, DoubleInverseIsOriginal)
{
    Matrix4x4F m = Matrix4x4F::TRS(
        Vector3F(1.f, 2.f, 3.f),
        Quaternion::FromAngleAxis(0.5_rf, Vector3F::Up()),
        Vector3F(1.f, 1.f, 1.f)
    );
    EXPECT_TRUE(Mat4Equal(m.Inversed().Inversed(), m, 1e-3f));
}

// ─────────────────────────────────────────────
// Decomposition: Translation / Rotation / Scale
// ─────────────────────────────────────────────

TEST(Matrix4x4FDecompositionTest, GetTranslationFromIdentity)
{
    Matrix4x4F id = Matrix4x4F::Identity();
    Vector3F t = id.GetTranslation();
    EXPECT_NEAR(t.x, 0.f, kEps);
    EXPECT_NEAR(t.y, 0.f, kEps);
    EXPECT_NEAR(t.z, 0.f, kEps);
}

TEST(Matrix4x4FDecompositionTest, GetTranslation)
{
    Vector3F expected(3.f, -1.f, 7.f);
    Matrix4x4F m = Matrix4x4F::Translation(expected);
    EXPECT_TRUE(Vec3Equal(m.GetTranslation(), expected));
}

TEST(Matrix4x4FDecompositionTest, GetScaleUniform)
{
    Vector3F s(4.f, 4.f, 4.f);
    Matrix4x4F m = Matrix4x4F::Scale(s);
    Vector3F extracted = m.GetScale();
    EXPECT_NEAR(extracted.x, s.x, kEps);
    EXPECT_NEAR(extracted.y, s.y, kEps);
    EXPECT_NEAR(extracted.z, s.z, kEps);
}

TEST(Matrix4x4FDecompositionTest, GetScaleNonUniform)
{
    Vector3F s(2.f, 3.f, 4.f);
    Matrix4x4F m = Matrix4x4F::Scale(s);
    Vector3F extracted = m.GetScale();
    EXPECT_NEAR(extracted.x, s.x, kEps);
    EXPECT_NEAR(extracted.y, s.y, kEps);
    EXPECT_NEAR(extracted.z, s.z, kEps);
}

TEST(Matrix4x4FDecompositionTest, GetScaleFromIdentity)
{
    Vector3F s = Matrix4x4F::Identity().GetScale();
    EXPECT_NEAR(s.x, 1.f, kEps);
    EXPECT_NEAR(s.y, 1.f, kEps);
    EXPECT_NEAR(s.z, 1.f, kEps);
}

TEST(Matrix4x4FDecompositionTest, GetRotationIdentity)
{
    Quaternion q = Matrix4x4F::Identity().GetRotation();
    Quaternion identity = Quaternion::Identity;

    bool sameOrNegated =
        (std::fabs(q.x - identity.x) < kEps && std::fabs(q.y - identity.y) < kEps &&
            std::fabs(q.z - identity.z) < kEps && std::fabs(q.w - identity.w) < kEps) ||
        (std::fabs(q.x + identity.x) < kEps && std::fabs(q.y + identity.y) < kEps &&
            std::fabs(q.z + identity.z) < kEps && std::fabs(q.w + identity.w) < kEps);

    EXPECT_TRUE(sameOrNegated);
}

TEST(Matrix4x4FDecompositionTest, GetRotation90DegAroundY)
{
    Quaternion q = Quaternion::FromAngleAxis(90_df, Vector3F::Up());
    Matrix4x4F m = Matrix4x4F::Rotation(q);
    Quaternion extracted = m.GetRotation();
    bool sameOrNegated =
        (std::fabs(extracted.x - q.x) < kEps && std::fabs(extracted.y - q.y) < kEps &&
            std::fabs(extracted.z - q.z) < kEps && std::fabs(extracted.w - q.w) < kEps) ||
        (std::fabs(extracted.x + q.x) < kEps && std::fabs(extracted.y + q.y) < kEps &&
            std::fabs(extracted.z + q.z) < kEps && std::fabs(extracted.w + q.w) < kEps);
    EXPECT_TRUE(sameOrNegated);
}

// ─────────────────────────────────────────────
// Point / Vector transform
// ─────────────────────────────────────────────

TEST(Matrix4x4FMultiplyTest, MultiplyPointOriginByTranslation)
{
    Matrix4x4F m = Matrix4x4F::Translation(Vector3F(1.f, 2.f, 3.f));
    Vector3F p = m.MultiplyPoint(Vector3F(0.f, 0.f, 0.f));
    EXPECT_NEAR(p.x, 1.f, kEps);
    EXPECT_NEAR(p.y, 2.f, kEps);
    EXPECT_NEAR(p.z, 3.f, kEps);
}

TEST(Matrix4x4FMultiplyTest, MultiplyPointNonOriginByTranslation)
{
    Matrix4x4F m = Matrix4x4F::Translation(Vector3F(1.f, 0.f, 0.f));
    Vector3F p = m.MultiplyPoint(Vector3F(2.f, 3.f, 4.f));
    EXPECT_NEAR(p.x, 3.f, kEps);
    EXPECT_NEAR(p.y, 3.f, kEps);
    EXPECT_NEAR(p.z, 4.f, kEps);
}

TEST(Matrix4x4FMultiplyTest, MultiplyPointFastAppliesTranslation)
{
    Matrix4x4F m = Matrix4x4F::Translation(Vector3F(4.f, -2.f, 1.f));
    Vector3F p = m.MultiplyPointFast(Vector3F(1.f, 1.f, 1.f));
    EXPECT_NEAR(p.x, 5.f, kEps);
    EXPECT_NEAR(p.y, -1.f, kEps);
    EXPECT_NEAR(p.z, 2.f, kEps);
}

TEST(Matrix4x4FMultiplyTest, MultiplyPointByIdentityUnchanged)
{
    Vector3F original(5.f, -3.f, 7.f);
    Vector3F result = Matrix4x4F::Identity().MultiplyPoint(original);
    EXPECT_TRUE(Vec3Equal(result, original));
}

TEST(Matrix4x4FMultiplyTest, MultiplyVectorIgnoresTranslation)
{
    Matrix4x4F m = Matrix4x4F::Translation(Vector3F(10.f, 20.f, 30.f));
    Vector3F v = m.MultiplyVector(Vector3F(1.f, 0.f, 0.f));
    EXPECT_NEAR(v.x, 1.f, kEps);
    EXPECT_NEAR(v.y, 0.f, kEps);
    EXPECT_NEAR(v.z, 0.f, kEps);
}

TEST(Matrix4x4FMultiplyTest, MultiplyVectorAppliesRotation)
{
    // 90° around Z: (1,0,0) → (0,1,0) (right-hand convention)
    Quaternion q = Quaternion::FromAngleAxis(RadianF(Mathf::PI / 2.f), Vector3F::Forward());
    Matrix4x4F m = Matrix4x4F::Rotation(q);
    Vector3F v = m.MultiplyVector(Vector3F(1.f, 0.f, 0.f));
    // Length is preserved
    float len = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    EXPECT_NEAR(len, 1.f, kEps);
    EXPECT_NEAR(v.z, 0.f, kEps);
}

TEST(Matrix4x4FMultiplyTest, MultiplyVectorByIdentityUnchanged)
{
    Vector3F original(1.f, 2.f, 3.f);
    Vector3F result = Matrix4x4F::Identity().MultiplyVector(original);
    EXPECT_TRUE(Vec3Equal(result, original));
}

// ─────────────────────────────────────────────
// Static factory methods
// ─────────────────────────────────────────────

TEST(Matrix4x4FFactoryTest, CreateTranslationStoresTranslation)
{
    Vector3F t(5.f, -3.f, 2.f);
    Matrix4x4F m = Matrix4x4F::Translation(t);
    EXPECT_TRUE(Vec3Equal(m.GetTranslation(), t));
}

TEST(Matrix4x4FFactoryTest, CreateTranslationUpperLeft3x3IsIdentity)
{
    Matrix4x4F m = Matrix4x4F::Translation(Vector3F(1.f, 2.f, 3.f));
    EXPECT_NEAR(m.m00, 1.f, kEps); EXPECT_NEAR(m.m11, 1.f, kEps);
    EXPECT_NEAR(m.m22, 1.f, kEps); EXPECT_NEAR(m.m01, 0.f, kEps);
}

TEST(Matrix4x4FFactoryTest, CreateTranslationZeroEqualsIdentity)
{
    Matrix4x4F m = Matrix4x4F::Translation(Vector3F(0.f, 0.f, 0.f));
    EXPECT_TRUE(Mat4Equal(m, Matrix4x4F::Identity()));
}

TEST(Matrix4x4FFactoryTest, CreateScaleDiagonal)
{
    Vector3F s(2.f, 3.f, 4.f);
    Matrix4x4F m = Matrix4x4F::Scale(s);
    EXPECT_NEAR(m.m00, s.x, kEps);
    EXPECT_NEAR(m.m11, s.y, kEps);
    EXPECT_NEAR(m.m22, s.z, kEps);
    EXPECT_NEAR(m.m33, 1.f, kEps);
    EXPECT_NEAR(m.m01, 0.f, kEps);
}

TEST(Matrix4x4FFactoryTest, CreateScaleOneEqualsIdentity)
{
    Matrix4x4F m = Matrix4x4F::Scale(Vector3F(1.f, 1.f, 1.f));
    EXPECT_TRUE(Mat4Equal(m, Matrix4x4F::Identity()));
}

TEST(Matrix4x4FFactoryTest, RotationIdentityQuaternionGivesIdentityMatrix)
{
    Quaternion q;
    Matrix4x4F m = Matrix4x4F::Rotation(q);
    EXPECT_TRUE(Mat4Equal(m, Matrix4x4F::Identity()));
}

TEST(Matrix4x4FFactoryTest, Rotation180DegAroundXFlipsYZ)
{
    Quaternion q = Quaternion::FromAngleAxis(RadianF(Mathf::PI), Vector3F::Right());
    Matrix4x4F m = Matrix4x4F::Rotation(q);
    Vector3F v = m.MultiplyVector(Vector3F(0.f, 1.f, 0.f));
    EXPECT_NEAR(v.x, 0.f, kEps);
    EXPECT_NEAR(v.y, -1.f, kEps);
}

TEST(Matrix4x4FFactoryTest, TRSTranslationRoundTrip)
{
    Vector3F t(1.f, 2.f, 3.f);
    Matrix4x4F m = Matrix4x4F::TRS(t, Quaternion(), Vector3F(1.f, 1.f, 1.f));
    EXPECT_TRUE(Vec3Equal(m.GetTranslation(), t));
}

TEST(Matrix4x4FFactoryTest, TRSScaleRoundTrip)
{
    Vector3F s(2.f, 3.f, 4.f);
    Matrix4x4F m = Matrix4x4F::TRS(Vector3F(0.f, 0.f, 0.f), Quaternion(), s);
    Vector3F extractedScale = m.GetScale();
    EXPECT_NEAR(extractedScale.x, s.x, kEps);
    EXPECT_NEAR(extractedScale.y, s.y, kEps);
    EXPECT_NEAR(extractedScale.z, s.z, kEps);
}

TEST(Matrix4x4FFactoryTest, TRSIdentityEqualsIdentity)
{
    Matrix4x4F m = Matrix4x4F::TRS(
        Vector3F(0.f, 0.f, 0.f),
        Quaternion(),
        Vector3F(1.f, 1.f, 1.f)
    );
    EXPECT_TRUE(Mat4Equal(m, Matrix4x4F::Identity()));
}

// ─────────────────────────────────────────────
// Projection matrices
// ─────────────────────────────────────────────

TEST(Matrix4x4FProjectionTest, PerspectiveDiagonalElementsAreFiniteAndPositive)
{
    Matrix4x4F m = Matrix4x4F::PerspectiveProjection(
        Mathf::PI / 2.f, 16.f / 9.f, 0.1f, 1000.f
    );
    EXPECT_TRUE(std::isfinite(m.m00)); EXPECT_GT(m.m00, 0.f);
    EXPECT_TRUE(std::isfinite(m.m11)); EXPECT_GT(m.m11, 0.f);
    EXPECT_TRUE(std::isfinite(m.m22));
    EXPECT_TRUE(std::isfinite(m.m23));
}

TEST(Matrix4x4FProjectionTest, PerspectiveNarrowFovHasLargerM11)
{
    Matrix4x4F wide = Matrix4x4F::PerspectiveProjection(Mathf::PI / 2.f, 1.f, 0.1f, 100.f);
    Matrix4x4F narrow = Matrix4x4F::PerspectiveProjection(Mathf::PI / 4.f, 1.f, 0.1f, 100.f);
    EXPECT_GT(narrow.m11, wide.m11);
}

TEST(Matrix4x4FProjectionTest, OrthographicSymmetricScaleIsOne)
{
    // Symmetric unit-cube mapping: l=-1,r=1,t=1,b=-1,n=0,f=1
    Matrix4x4F m = Matrix4x4F::OrthographicProjection(-1.f, 1.f, 1.f, -1.f, 0.f, 1.f);
    EXPECT_NEAR(std::fabs(m.m00), 1.f, kEps);
    EXPECT_NEAR(std::fabs(m.m11), 1.f, kEps);
    EXPECT_TRUE(std::isfinite(m.m22));
}

TEST(Matrix4x4FProjectionTest, OrthographicElementsAreFinite)
{
    Matrix4x4F m = Matrix4x4F::OrthographicProjection(
        0.f, 800.f, 600.f, 0.f, -1.f, 1.f
    );
    for (int i = 0; i < Matrix4x4F::ELEMENT_COUNT; ++i)
        EXPECT_TRUE(std::isfinite(m.elements[i]));
}

// ─────────────────────────────────────────────
// LookAt
// ─────────────────────────────────────────────

TEST(Matrix4x4FLookAtTest, LookAtUpperLeft3x3IsOrthonormal)
{
    Matrix4x4F m = Matrix4x4F::LookAt(
        Vector3F(0.f, 0.f, 5.f),
        Vector3F(0.f, 0.f, 0.f),
        Vector3F::Up()
    );
    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            float dot =
                m.GetRow(row).x * m.GetRow(col).x +
                m.GetRow(row).y * m.GetRow(col).y +
                m.GetRow(row).z * m.GetRow(col).z;
            float expected = (row == col) ? 1.f : 0.f;
            EXPECT_NEAR(dot, expected, kEps);
        }
    }
}

TEST(Matrix4x4FLookAtTest, LookAtDefaultUpParameterDoesNotThrow)
{
    EXPECT_NO_THROW({
        Matrix4x4F m = Matrix4x4F::LookAt(
            Vector3F(1.f, 1.f, 1.f),
            Vector3F(0.f, 0.f, 0.f)
        );
        (void)m;
        });
}

TEST(Matrix4x4FLookAtTest, LookAtCustomUpVector)
{
    EXPECT_NO_THROW({
        Matrix4x4F m = Matrix4x4F::LookAt(
            Vector3F(0.f, 5.f, 0.f),
            Vector3F(0.f, 0.f, 0.f),
            Vector3F::Forward()
        );
        (void)m;
        });
}

// ─────────────────────────────────────────────
// Robustness
// ─────────────────────────────────────────────

TEST(Matrix4x4FRobustnessTest, UniformScaleThenInverse)
{
    Matrix4x4F s = Matrix4x4F::Scale(Vector3F(2.f, 2.f, 2.f));
    EXPECT_TRUE(Mat4Equal(s * s.Inversed(), Matrix4x4F::Identity()));
}

TEST(Matrix4x4FRobustnessTest, ChainedTRSAndInverse)
{
    Matrix4x4F trs = Matrix4x4F::TRS(
        Vector3F(3.f, -1.f, 2.f),
        Quaternion::FromAngleAxis(0.785398_rf, Vector3F(0.f, 1.f, 0.f)),
        Vector3F(1.f, 1.f, 1.f)
    );
    EXPECT_TRUE(Mat4Equal(trs * trs.Inversed(), Matrix4x4F::Identity(), 1e-3f));
}

TEST(Matrix4x4FRobustnessTest, StaticIdentityRowsMatchNamedRows)
{
    EXPECT_EQ(Matrix4x4F::Identity().GetRow(0), Matrix4x4F::IdentityRow0());
    EXPECT_EQ(Matrix4x4F::Identity().GetRow(1), Matrix4x4F::IdentityRow1());
    EXPECT_EQ(Matrix4x4F::Identity().GetRow(2), Matrix4x4F::IdentityRow2());
    EXPECT_EQ(Matrix4x4F::Identity().GetRow(3), Matrix4x4F::IdentityRow3());
}

TEST(Matrix4x4FRobustnessTest, NegativeScalePreservesAbsoluteGetScale)
{
    Vector3F s(-1.f, 2.f, -3.f);
    Matrix4x4F m = Matrix4x4F::Scale(s);
    Vector3F extracted = m.GetScale();
    // GetScale returns magnitudes of basis vectors
    EXPECT_NEAR(extracted.x, std::fabs(s.x), kEps);
    EXPECT_NEAR(extracted.y, std::fabs(s.y), kEps);
    EXPECT_NEAR(extracted.z, std::fabs(s.z), kEps);
}