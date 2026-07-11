#include <DirectXMath.h> 

#include "ByteEngine/Math/Matrix4x4F.h"
#include "ByteEngine/Math/Quaternion.h"

using namespace DirectX;

namespace ByteEngine::Math
{
    const Matrix4x4F Matrix4x4F::Identity { 
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1 
    };

    float Matrix4x4F::Determinant() const
    {
        XMMATRIX matrix = XMLoadFloat4x4A(reinterpret_cast<const XMFLOAT4X4A*>(this->elements));
        XMVECTOR det = XMMatrixDeterminant(matrix);
        return XMVectorGetX(det);
    }

    void Matrix4x4F::Inverse()
    {
        XMMATRIX matrix = XMLoadFloat4x4A(reinterpret_cast<const XMFLOAT4X4A*>(this->elements));
        XMMATRIX invMatrix = XMMatrixInverse(nullptr, matrix);
        XMStoreFloat4x4A(reinterpret_cast<XMFLOAT4X4A*>(this->elements), invMatrix);
    }

    Matrix4x4F Matrix4x4F::Inversed() const
    {
        XMMATRIX matrix = XMLoadFloat4x4A(reinterpret_cast<const XMFLOAT4X4A*>(this->elements));
        XMMATRIX invMatrix = XMMatrixInverse(nullptr, matrix);

        Matrix4x4F result;
        XMStoreFloat4x4A(reinterpret_cast<XMFLOAT4X4A*>(&result), invMatrix);
        return result;
    }

    Quaternion Matrix4x4F::GetRotation() const
    {
        XMMATRIX matrix = XMLoadFloat4x4A(reinterpret_cast<const XMFLOAT4X4A*>(this->elements));

        XMVECTOR x = matrix.r[0];
        XMVECTOR y = matrix.r[1];
        XMVECTOR z = matrix.r[2];

        x = XMVector3Normalize(x);

        XMVECTOR dotYX = XMVector3Dot(y, x);
        y = XMVectorSubtract(y, XMVectorMultiply(x, dotYX));
        y = XMVector3Normalize(y);

        XMVECTOR dotZX = XMVector3Dot(z, x);
        XMVECTOR dotZY = XMVector3Dot(z, y);
        z = XMVectorSubtract(z, XMVectorMultiply(x, dotZX));
        z = XMVectorSubtract(z, XMVectorMultiply(y, dotZY));
        z = XMVector3Normalize(z);

        XMMATRIX orthoMatrix;
        orthoMatrix.r[0] = XMVectorSelect(g_XMIdentityR3, x, g_XMSelect1110);
        orthoMatrix.r[1] = XMVectorSelect(g_XMIdentityR3, y, g_XMSelect1110);
        orthoMatrix.r[2] = XMVectorSelect(g_XMIdentityR3, z, g_XMSelect1110);
        orthoMatrix.r[3] = g_XMIdentityR3;

        XMVECTOR quat = XMQuaternionRotationMatrix(orthoMatrix);
        Quaternion result;        
        XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result), quat);

        return result;
    }

    Vector3F Matrix4x4F::GetScale() const
    {
        XMMATRIX matrix = XMLoadFloat4x4A(reinterpret_cast<const XMFLOAT4X4A*>(this->elements));

        XMVECTOR x = XMVector3Length(matrix.r[0]);
        XMVECTOR y = XMVector3Length(matrix.r[1]);
        XMVECTOR z = XMVector3Length(matrix.r[2]);

        Vector3F scale;

        XMVectorGetXPtr(&scale.x, x);
        XMVectorGetXPtr(&scale.y, y);
        XMVectorGetXPtr(&scale.z, z);

        return scale;
    }

    Vector3F Matrix4x4F::MultiplyPoint(Vector3F point) const
    {
        XMVECTOR vec = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&point));
        XMMATRIX matrix = XMLoadFloat4x4A(reinterpret_cast<const XMFLOAT4X4A*>(this->elements));
        XMVECTOR transformedPoint = XMVector3TransformCoord(vec, matrix);

        Vector3F result;
        XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&result), transformedPoint);
        return result;
    }

    Vector3F Matrix4x4F::MultiplyPointFast(Vector3F point) const
    {
        XMVECTOR vec = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&point));
        XMMATRIX matrix = XMLoadFloat4x4A(reinterpret_cast<const XMFLOAT4X4A*>(this->elements));
        XMVECTOR transformedPoint = XMVector3Transform(vec, matrix);

        Vector3F result;
        XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&result), transformedPoint);
        return result;
    }

    Vector3F Matrix4x4F::MultiplyVector(Vector3F vector) const
    {
        XMVECTOR vec = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&vector));
        XMMATRIX matrix = XMLoadFloat4x4A(reinterpret_cast<const XMFLOAT4X4A*>(this->elements));
        XMVECTOR transformedPoint = XMVector3TransformNormal(vec, matrix);

        Vector3F result;
        XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&result), transformedPoint);
        return result;
    }

    Matrix4x4F Matrix4x4F::CreateRotation(Quaternion quat)
    {
        XMVECTOR quatVec = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&quat));
        XMMATRIX rotMatrix = XMMatrixRotationQuaternion(quatVec);

        Matrix4x4F result;
        XMStoreFloat4x4A(reinterpret_cast<XMFLOAT4X4A*>(&result), rotMatrix);
        return result;
    }    

    Matrix4x4F Matrix4x4F::CreatePerspectiveProjection(float fovY, float aspectRatio, float nearPlane, float farPlane)
    {
        XMMATRIX matrix = XMMatrixPerspectiveFovLH(fovY, aspectRatio, nearPlane, farPlane);
        Matrix4x4F result;
        XMStoreFloat4x4A(reinterpret_cast<XMFLOAT4X4A*>(&result), matrix);
        return result;
    }

    Matrix4x4F Matrix4x4F::CreateOrthographicProjection(float left, float right, float top, float bottom, float nearPlane, float farPlane)
    {
        XMMATRIX matrix = XMMatrixOrthographicOffCenterLH(left, right, bottom, top, nearPlane, farPlane);
        Matrix4x4F result;
        XMStoreFloat4x4A(reinterpret_cast<XMFLOAT4X4A*>(&result), matrix);
        return result;
    }

    Matrix4x4F Matrix4x4F::CreateLookAt(Vector3F eyePos, Vector3F targetPos, Vector3F worldUp)
    {
        Vector3F direction = Vector3F::Direction(eyePos, targetPos);

        XMVECTOR eyeVec = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&eyePos));
        XMVECTOR dirVec = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&direction));
        XMVECTOR upVec = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&worldUp));

        XMMATRIX viewMatrix = XMMatrixLookToLH(eyeVec, dirVec, upVec);
        Matrix4x4F result;
        XMStoreFloat4x4A(reinterpret_cast<XMFLOAT4X4A*>(&result), viewMatrix);
        return result;
    }

    Matrix4x4F Matrix4x4F::CreateTRS(Vector3F translation, Quaternion rotation, Vector3F scale)
    {
        XMVECTOR translationVec = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&translation));
        XMVECTOR rotationVec = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&rotation));

        XMMATRIX translationMatrix = XMMatrixTranslationFromVector(translationVec);
        XMMATRIX rotationMatrix = XMMatrixRotationQuaternion(rotationVec);
        XMMATRIX scaleMatrix = XMMatrixScaling(scale.x, scale.y, scale.z);

        XMMATRIX trsMatrix = scaleMatrix * rotationMatrix * translationMatrix;

        Matrix4x4F result;
        XMStoreFloat4x4A(reinterpret_cast<XMFLOAT4X4A*>(&result), trsMatrix);
        return result;
    }

    Matrix4x4F Matrix4x4F::operator*(const Matrix4x4F& other) const
    {
        XMMATRIX matrixA = XMLoadFloat4x4A(reinterpret_cast<const XMFLOAT4X4A*>(this->elements));
        XMMATRIX matrixB = XMLoadFloat4x4A(reinterpret_cast<const XMFLOAT4X4A*>(other.elements));

        XMMATRIX resultMatrix = matrixA * matrixB;

        Matrix4x4F result;
        XMStoreFloat4x4A(reinterpret_cast<XMFLOAT4X4A*>(&result), resultMatrix);
        return result;
    }

    Matrix4x4F& Matrix4x4F::operator*=(const Matrix4x4F& other)
    {
        *this = *this * other;
        return *this;
    }
}