#include "ByteEngine/Utilities/Utils.h"

#include "Platform/Core/Graphics/GraphicsDeviceD3D11.h"

using namespace ByteEngine;
using namespace ByteEngine::Graphics;

GraphicsDeviceD3D11::Error GraphicsDeviceD3D11::Initialize(bool enableDebug)
{
    ComPtr<ID3D11Device> baseDevice;
    D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_1 };
    uint32 creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

    if (enableDebug)
        creationFlags |= D3D11_CREATE_DEVICE_DEBUG;

    HRESULT hResult = D3D11CreateDevice(
        0, D3D_DRIVER_TYPE_HARDWARE,
        0, creationFlags,
        featureLevels, Utils::ArraySize(featureLevels),
        D3D11_SDK_VERSION, baseDevice.put(),
        nullptr, nullptr
    );
    
    if (FAILED(hResult))
    {
        if (hResult == DXGI_ERROR_UNSUPPORTED)
            return Error::Unsupported;
        else
            return Error::GenericError;
    }

    hResult = baseDevice->QueryInterface(IID_PPV_ARGS(&device));
    if (FAILED(hResult))
    {
        if (hResult == E_NOINTERFACE)
            return Error::Unsupported;
        else
            return Error::GenericError;
    }

    if (enableDebug)
        CreateDebugLayer();

    return Error::Success;
}

void GraphicsDeviceD3D11::CreateDebugLayer()
{
    ComPtr<ID3D11Debug> debug = nullptr;
    HRESULT hResult = device->QueryInterface(IID_PPV_ARGS(debug.put()));

    if (FAILED(hResult))
        return;

    ComPtr<ID3D11InfoQueue> infoQueue = nullptr;
    hResult = debug->QueryInterface(IID_PPV_ARGS(infoQueue.put()));

    if (FAILED(hResult))
        return;

    infoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
    infoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
}
