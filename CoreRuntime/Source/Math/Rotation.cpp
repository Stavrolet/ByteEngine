#include "ByteEngine/Math/Rotation.h"

namespace ByteEngine::Math
{
    void Rotation::Normalize()
    {
        pitch = DegreeF { Mathf::WrapValue(pitch.value, -180.0f, 180.0f) };
        yaw = DegreeF { Mathf::WrapValue(yaw.value, -180.0f, 180.0f) };
        roll = DegreeF { Mathf::WrapValue(roll.value, -180.0f, 180.0f) };
    }

    Rotation Rotation::Normalized() const
    {
        Rotation copy = *this;
        copy.Normalize();
        return copy;
    }

    Quaternion Rotation::ToQuaternion() const { return Quaternion::FromEuler(pitch, yaw, roll); }

    Vector3F Rotation::RotateVector(Vector3F vector) const
    {
        Quaternion q = ToQuaternion();
        return q * vector;
    }

    Rotation Rotation::FromQuaternion(Quaternion q)
    {
        EulerDeg euler = q.GetEulerInDegrees();
        return Rotation(euler);
    }

    bool Rotation::IsEqualApproximately(Rotation a, Rotation b, DegreeF epsilon)
    {
        return Mathf::IsEqualApproximetly(a.pitch.value, b.pitch.value, epsilon.value) &&
            Mathf::IsEqualApproximetly(a.yaw.value, b.yaw.value, epsilon.value) &&
            Mathf::IsEqualApproximetly(a.roll.value, b.roll.value, epsilon.value);
    }

    Rotation Rotation::Slerp(Rotation from, Rotation to, float t)
    {
        Quaternion qFrom = from.ToQuaternion();
        Quaternion qTo = to.ToQuaternion();
        Quaternion qResult = Quaternion::Slerp(qFrom, qTo, t);
        return FromQuaternion(qResult);
    }

    Rotation Rotation::SlerpClamped(Rotation from, Rotation to, float t)
    {
        return Slerp(from, to, Mathf::Clamp(t));
    }
}