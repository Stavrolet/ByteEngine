#include <DirectXMath.h>

#include "ByteEngine/Math/Vector2.h"
#include "ByteEngine/Math/Quaternion.h"

using namespace DirectX;

namespace ByteEngine::Math
{
    const Quaternion Quaternion::Identity { 0.0f, 0.0f, 0.0f, 1.0f };

    float Quaternion::Length() const { return Mathf::Sqrt(LengthSquared()); }

    void Quaternion::Normalize()
    {
        float length = LengthSquared();

        if (length > Mathf::Epsilon)
        {
            float invLength = 1.0f / Mathf::Sqrt(length);
            x *= invLength;
            y *= invLength;
            z *= invLength;
            w *= invLength;
        }
    }

    Quaternion Quaternion::Normalized() const
    {
        Quaternion copy = *this;
        copy.Normalize();
        return copy;
    }

    EulerRad Quaternion::GetEuler()
    {
        if (!IsNormalized())
            Normalize();

        float num = w * w - x * x;
        float ySq = y * y;
        float zSq = z * z;

        return EulerRad { Mathf::Asin(2 * (w * x - y * z)), Mathf::Atan2(2 * (w * y + x * z), num + ySq - zSq), Mathf::Atan2(2 * (w * z + x * y), num - ySq + zSq) };
    }

    EulerRad Quaternion::GetEuler() const
    {
        BE_ASSERT(IsNormalized());

        float sinp = 2.0f * (w * x - z * y);

        float siny = 2.0f * (w * y + x * z);
        float cosy = 1.0f - 2.0f * (x * x + y * y);

        float sinr = 2.0f * (w * z + x * y);
        float cosr = 1.0f - 2.0f * (x * x + z * z);

        return EulerRad { Mathf::Asin(sinp), Mathf::Atan2(siny, cosy), Mathf::Atan2(sinr, cosr) };
    }

    EulerDeg Quaternion::GetEulerInDegrees() const
    {
        EulerRad euler = GetEuler();
        return euler.ToDeg();
    }

    // GetAxis implementation adapted from Godot Engine (MIT License). See THIRDPARTY.md
    // Source: Quaternion::get_axis
    Vector3F Quaternion::GetAxis() const
    {
        if (Mathf::Abs(w) > 1 - Mathf::Epsilon)
            return Vector3F(x, y, z);

        float invRoot = 1.0f / Mathf::Sqrt(1 - w * w);
        return Vector3F(x * invRoot, y * invRoot, z * invRoot);
    }

    // GetAngle implementation adapted from Godot Engine (MIT License). See THIRDPARTY.md
    // Source: Quaternion::get_angle
    RadianF Quaternion::GetAngle() const { return 2 * Mathf::Acos(w); }

    RadianF Quaternion::AngleBetween(Quaternion a, Quaternion b) { return 2 * Mathf::Acos(Mathf::Abs(Dot(a, b))); }

    // FromAngleAxis implementation adapted from Godot Engine (MIT License). See THIRDPARTY.md
    // Source: Quaternion::Quaternion(const Vector3f &p_axis, real_t p_angle)
    Quaternion Quaternion::FromAngleAxis(RadianF angle, Vector3F axis)
    {
        if (!axis.IsNormalized())
            axis.Normalize();

        float length = axis.Length();

        if (Mathf::IsEqualApproximetly(length, 0.0f))
        {
            return Quaternion(0.0f);
        }
        else
        {
            float sin, cos;
            Mathf::SinCos(sin, cos, angle * 0.5f);

            float s = sin / length;
            return Quaternion(axis.x * s, axis.y * s, axis.z * s, cos);
        }
    }

    Quaternion Quaternion::FromAngleAxis(DegreeF angle, Vector3F axis) { return FromAngleAxis(angle.ToRadian(), axis); }

    Quaternion Quaternion::FromEuler(RadianF pitch, RadianF yaw, RadianF roll)
    {
        XMVECTOR q = XMQuaternionRotationRollPitchYaw(pitch.value, yaw.value, roll.value);
        Quaternion result;
        XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result), q);
        return result;
    }

    Quaternion Quaternion::FromEuler(DegreeF pitch, DegreeF yaw, DegreeF roll) { return FromEuler(pitch.ToRadian(), yaw.ToRadian(), roll.ToRadian()); }

    Quaternion Quaternion::FromLookDirection(Vector3F direction, Vector3F worldUp)
    {
        if (Mathf::IsEqualApproximetly(direction.LengthSquared(), 0.0f))
            return Identity;

        XMVECTOR forward = XMLoadFloat3(reinterpret_cast<XMFLOAT3*>(&direction));
        XMVECTOR worldUp2 = XMLoadFloat3(reinterpret_cast<XMFLOAT3*>(&worldUp));

        if (!direction.IsNormalized())
            forward = XMVector3Normalize(forward);

        XMVECTOR right = XMVector3Cross(worldUp2, forward);
        XMVECTOR up = XMVector3Cross(forward, right);

        XMMATRIX m { right, up, forward, g_XMIdentityR3 };

        XMVECTOR q = XMQuaternionRotationMatrix(m);
        Quaternion result;
        XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result), q);
        return result;
    }

    Quaternion Quaternion::FromToRotation(Vector3F from, Vector3F to)
    {
        from.Normalize();
        to.Normalize();

        float dot = Mathf::Clamp(Vector3F::Dot(from, to), -1.0f, 1.0f);

        if (dot >= 1.0f || Mathf::IsEqualApproximetly(dot, 0.0f))
            return Identity;
        else if (dot <= -1.0f)
            return FromAngleAxis(static_cast<RadianF>(Mathf::PI), Vector3F(0.0f, 1.0f, 0.0f));

        Vector3F axis = Vector3F::Cross(from, to);
        float root = Mathf::Sqrt((1.0f + dot) * 2.0f);
        float invRoot = 1.0f / root;

        return Quaternion(axis.x * invRoot, axis.y * invRoot, axis.z * invRoot, root * 0.5f);
    }

    // SlerpUnclamped implementation adapted from Godot Engine (MIT License). See THIRDPARTY.md
    // Source: Quaternion::slerp
    Quaternion Quaternion::Slerp(Quaternion from, Quaternion to, float t)
    {
        BE_ASSERT(from.IsNormalized());
        BE_ASSERT(to.IsNormalized());

        Quaternion to1;
        RadianF omega;
        float cosom, sinom, scale0, scale1;

        cosom = Dot(from, to);

        if (cosom < 0.0f)
        {
            cosom = -cosom;
            to1 = -to;
        }
        else
        {
            to1 = to;
        }

        if ((1.0f - cosom) > Mathf::Epsilon)
        {
            omega = Mathf::Acos(cosom);
            sinom = Mathf::Sin(omega);
            scale0 = Mathf::Sin(static_cast<RadianF>((1.0f - t) * omega)) / sinom;
            scale1 = Mathf::Sin(static_cast<RadianF>(t * omega)) / sinom;
        }
        else
        {
            scale0 = 1.0f - t;
            scale1 = t;
        }

        return Quaternion(
            scale0 * from.x + scale1 * to1.x,
            scale0 * from.y + scale1 * to1.y,
            scale0 * from.z + scale1 * to1.z,
            scale0 * from.w + scale1 * to1.w
        );
    }

    Quaternion Quaternion::SlerpClamped(Quaternion from, Quaternion to, float t)
    {
        return Slerp(from, to, Mathf::Clamp(t));
    }

    bool Quaternion::IsEqualApproximetly(Quaternion a, Quaternion b, float tolerance)
    {
        return Mathf::IsEqualApproximetly(a.x, b.x, tolerance) && Mathf::IsEqualApproximetly(a.y, b.y, tolerance) && Mathf::IsEqualApproximetly(a.z, b.z, tolerance) && Mathf::IsEqualApproximetly(a.w, b.w, tolerance);
    }
}