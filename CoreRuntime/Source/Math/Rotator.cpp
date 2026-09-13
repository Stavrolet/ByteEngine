#include "ByteEngine/Math/Rotator.h"

namespace ByteEngine::Math
{
    void Rotator::Normalize()
    {
        pitch = DegreeF { Mathf::WrapValue(pitch.value, -180.0f, 180.0f) };
        yaw = DegreeF { Mathf::WrapValue(yaw.value, -180.0f, 180.0f) };
        roll = DegreeF { Mathf::WrapValue(roll.value, -180.0f, 180.0f) };
    }

    Rotator Rotator::Normalized() const
    {
        Rotator copy = *this;
        copy.Normalize();
        return copy;
    }

    Vector3F Rotator::RotateVector(Vector3F vector) const
    {
        Quaternion q = Quaternion(*this);
        return q * vector;
    }

    bool Rotator::IsEqualApproximately(Rotator a, Rotator b, DegreeF epsilon)
    {
        return Mathf::IsEqualApproximetly(a.pitch.value, b.pitch.value, epsilon.value) &&
            Mathf::IsEqualApproximetly(a.yaw.value, b.yaw.value, epsilon.value) &&
            Mathf::IsEqualApproximetly(a.roll.value, b.roll.value, epsilon.value);
    }

    Rotator Rotator::Slerp(Rotator from, Rotator to, float t)
    {
        Quaternion qFrom = Quaternion(from);
        Quaternion qTo = Quaternion(to);
        Quaternion qResult = Quaternion::Slerp(qFrom, qTo, t);
        return Rotator(qResult);
    }

    Rotator Rotator::SlerpClamped(Rotator from, Rotator to, float t)
    {
        return Slerp(from, to, Mathf::Clamp(t));
    }
}