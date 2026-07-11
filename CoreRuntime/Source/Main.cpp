//#include <Windows.h>
//#include <strsafe.h>
//#include <d3d11.h>
//#include <d3dcompiler.h>
//#include <DirectXMath.h>
//#include <wrl/client.h>
//#include <WICTextureLoader.h>
//#include <string>
//#include <initializer_list>
//#include <format>
//#include "GameTime.h"
//#include "Core/Base/Application.h"
//#include "Math/Math.h"
//#include "Math/Quaternion.h"
//
//using namespace DirectX;
//using namespace ByteEngine;
//using Microsoft::WRL::ComPtr;
//
//constexpr bool FULLSCREEN = false;
//
//int ScreenWidth = 0;
//int ScreenHeight = 0;
//
//HCURSOR hCursor = NULL;
//bool isCursorVisible = true;
//
//XMVECTOR cameraPos = XMVectorSet(0.0f, 2.0f, -5.0f, 0.0f);
//XMVECTOR cameraRotation = XMQuaternionIdentity();
//float cameraFovAngle = 60.0f;
//
//XMVECTOR dimaScale = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
//XMVECTOR dimaRotation = XMQuaternionIdentity();
//float bendAmount = 0.5f;
//bool useWave = true;
//
//struct Vertex
//{
//    XMFLOAT3 Pos;
//    XMFLOAT2 Tex;
//};
//
//struct alignas(16) PerModelBuffer
//{
//    XMMATRIX ModelViewProjection;
//};
//
//struct alignas(16) PerFrameBuffer
//{
//    float Time;
//    float DeltaTime;
//    float BendAmount;
//    bool UseWave;
//};
//
//ComPtr<ID3D11Device> device;
//ComPtr<ID3D11DeviceContext> deviceContext;
//ComPtr<IDXGISwapChain> swapchain;
//
//ComPtr<ID3D11RenderTargetView> renderTarget;
//ComPtr<ID3D11DepthStencilView> depthStencilView;
//
//ComPtr<ID3D11VertexShader> cubeVertexShader;
//ComPtr<ID3D11VertexShader> planeVertexShader;
//ComPtr<ID3D11PixelShader> pixelShader;
//ComPtr<ID3D11InputLayout> inputLayout;
//
//ComPtr<ID3D11Buffer> cubeVertexBuffer;
//ComPtr<ID3D11Buffer> cubeIndexBuffer;
//ComPtr<ID3D11Buffer> planeVertexBuffer;
//ComPtr<ID3D11Buffer> planeIndexBuffer;
//
//ComPtr<ID3D11Buffer> perModelBuffer;
//ComPtr<ID3D11Buffer> perFrameBuffer;
//
//ComPtr<ID3D11ShaderResourceView> brickTextureSrv;
//ComPtr<ID3D11ShaderResourceView> dimaTextureSrv;
//ComPtr<ID3D11SamplerState> sampler;
//ComPtr<ID3D11RasterizerState> rasterizerState;
//
//Vertex cubeVertices[] = {
//    { { -1.0f, -1.0f, -1.0f }, { 0.0f, 1.0f } }, { { -1.0f, 1.0f, -1.0f }, { 0.0f, 0.0f } },
//    { { 1.0f, 1.0f, -1.0f }, { 1.0f, 0.0f } }, { { 1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f } },
//    { { -1.0f, -1.0f, 1.0f }, { 1.0f, 1.0f } }, { { 1.0f, -1.0f, 1.0f }, { 0.0f, 1.0f } },
//    { { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } }, { { -1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } },
//    { { -1.0f, 1.0f, -1.0f }, { 0.0f, 1.0f } }, { { -1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },
//    { { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } }, { { 1.0f, 1.0f, -1.0f }, { 1.0f, 1.0f } },
//    { { -1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f } }, { { 1.0f, -1.0f, -1.0f }, { 0.0f, 1.0f } },
//    { { 1.0f, -1.0f, 1.0f }, { 0.0f, 0.0f } }, { { -1.0f, -1.0f, 1.0f }, { 1.0f, 0.0f } },
//    { { -1.0f, -1.0f, 1.0f }, { 0.0f, 1.0f } }, { { -1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },
//    { { -1.0f, 1.0f, -1.0f }, { 1.0f, 0.0f } }, { { -1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f } },
//    { { 1.0f, -1.0f, -1.0f }, { 0.0f, 1.0f } }, { { 1.0f, 1.0f, -1.0f }, { 0.0f, 0.0f } },
//    { { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } }, { { 1.0f, -1.0f, 1.0f }, { 1.0f, 1.0f } },
//};
//
//uint16_t cubeIndices[] = {
//    0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7, 8, 9, 10, 8, 10, 11,
//    12, 13, 14, 12, 14, 15, 16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23
//};
//
//Vertex planeVertices[] =
//{
//    { { -0.5f, 0.0f, -1.0f }, { 0.0f, 0.0f } },
//    { { 0.0f, 0.0f, -1.0f }, { 0.5f, 0.0f } },
//    { { 0.5f, 0.0f, -1.0f }, { 1.0f, 0.0f } },
//    { { -0.5f, 0.0f, -0.5f }, { 0.0f, 0.25f } },
//    { { 0.0f, 0.0f, -0.5f }, { 0.5f, 0.25f } },
//    { { 0.5f, 0.0f, -0.5f }, { 1.0f, 0.25f } },
//    { { -0.5f, 0.0f, 0.0f }, { 0.0f, 0.5f } },
//    { { 0.0f, 0.0f, 0.0f }, { 0.5f, 0.5f } },
//    { { 0.5f, 0.0f, 0.0f }, { 1.0f, 0.5f } },
//    { { -0.5f, 0.0f, 0.5f }, { 0.0f, 0.75f } },
//    { { 0.0f, 0.0f, 0.5f }, { 0.5f, 0.75f } },
//    { { 0.5f, 0.0f, 0.5f }, { 1.0f, 0.75f } },
//    { { -0.5f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
//    { { 0.0f, 0.0f, 1.0f }, { 0.5f, 1.0f } },
//    { { 0.5f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
//};
//
//uint16_t planeIndicies[] =
//{
//    0, 3, 1, 1, 3, 4,
//    1, 4, 2, 2, 4, 5,
//    3, 6, 4, 4, 6, 7,
//    4, 7, 5, 5, 7, 8,
//    6, 9, 7, 7, 9, 10,
//    7, 10, 8, 8, 10, 11,
//    9, 12, 10, 10, 12, 13,
//    10, 13, 11, 11, 13, 14
//};
//
//static void InitD3D(HWND hwnd)
//{
//    DXGI_SWAP_CHAIN_DESC scd = { 0 };
//    scd.BufferCount = 1;
//    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//    scd.BufferDesc.Width = ScreenWidth;
//    scd.BufferDesc.Height = ScreenHeight;
//    scd.BufferDesc.RefreshRate.Numerator = 120;
//    scd.BufferDesc.RefreshRate.Denominator = 1;
//    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
//    scd.OutputWindow = hwnd;
//    scd.SampleDesc.Count = 1;
//    scd.Windowed = true;
//    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
//
//    //scd.Width = 0;
//    //scd.Height = 0;
//    //scd.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
//    //scd.SampleDesc.Count = 1;
//    //scd.SampleDesc.Quality = 0;
//    //scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
//    //scd.BufferCount = 2;
//    //scd.Scaling = DXGI_SCALING_ASPECT_RATIO_STRETCH;
//    //scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
//    //scd.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
//    //scd.Flags = 0;
//
//    HRESULT result1 = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0,
//        D3D11_SDK_VERSION, &scd, swapchain.GetAddressOf(), device.GetAddressOf(), nullptr, deviceContext.GetAddressOf());
//
//    swapchain->SetFullscreenState(FULLSCREEN, nullptr);
//
//    ComPtr<ID3D11Texture2D> backBuffer;
//    swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
//    device->CreateRenderTargetView(backBuffer.Get(), nullptr, renderTarget.GetAddressOf());
//
//    D3D11_TEXTURE2D_DESC depthDesc = { 0 };
//    depthDesc.Width = ScreenWidth;
//    depthDesc.Height = ScreenHeight;
//    depthDesc.MipLevels = 1;
//    depthDesc.ArraySize = 1;
//    depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
//    depthDesc.SampleDesc.Count = 1;
//    depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
//
//    ComPtr<ID3D11Texture2D> depthStencilBuffer;
//    device->CreateTexture2D(&depthDesc, nullptr, depthStencilBuffer.GetAddressOf());
//    device->CreateDepthStencilView(depthStencilBuffer.Get(), nullptr, depthStencilView.GetAddressOf());
//
//    deviceContext->OMSetRenderTargets(1, renderTarget.GetAddressOf(), depthStencilView.Get());
//
//    D3D11_VIEWPORT viewport = { 0, 0, (float)ScreenWidth, (float)ScreenHeight, 0, 1 };
//    deviceContext->RSSetViewports(1, &viewport);
//
//    ComPtr<ID3DBlob> cubeVertexShaderBlob, pixelShaderBlob, planeVertexShaderBlob;
//
//    D3DReadFileToBlob(LR"(E:\Work Space\C++ Projects\ByteEngineD3D11\x64\Debug\Shaders\CubeVertexShader.cso)", cubeVertexShaderBlob.GetAddressOf());
//    D3DReadFileToBlob(LR"(E:\Work Space\C++ Projects\ByteEngineD3D11\x64\Debug\Shaders\PixelShader.cso)", pixelShaderBlob.GetAddressOf());
//    D3DReadFileToBlob(LR"(E:\Work Space\C++ Projects\ByteEngineD3D11\x64\Debug\Shaders\PlaneVertexShader.cso)", planeVertexShaderBlob.GetAddressOf());
//
//    device->CreateVertexShader(cubeVertexShaderBlob->GetBufferPointer(), cubeVertexShaderBlob->GetBufferSize(), nullptr, cubeVertexShader.GetAddressOf());
//    device->CreateVertexShader(planeVertexShaderBlob->GetBufferPointer(), planeVertexShaderBlob->GetBufferSize(), nullptr, planeVertexShader.GetAddressOf());
//    device->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, pixelShader.GetAddressOf());
//
//    D3D11_INPUT_ELEMENT_DESC ied[] = {
//        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//    };
//
//    device->CreateInputLayout(ied, 2, cubeVertexShaderBlob->GetBufferPointer(), cubeVertexShaderBlob->GetBufferSize(), inputLayout.GetAddressOf());
//    deviceContext->IASetInputLayout(inputLayout.Get());
//
//    D3D11_BUFFER_DESC bd = { 0 };
//    bd.Usage = D3D11_USAGE_DEFAULT;
//    bd.ByteWidth = sizeof(cubeVertices);
//    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//
//    D3D11_SUBRESOURCE_DATA initData = { cubeVertices, 0, 0 };
//    device->CreateBuffer(&bd, &initData, cubeVertexBuffer.GetAddressOf());
//
//    bd.ByteWidth = sizeof(planeVertices);
//    initData.pSysMem = planeVertices;
//    device->CreateBuffer(&bd, &initData, planeVertexBuffer.GetAddressOf());
//
//    bd.ByteWidth = sizeof(cubeIndices);
//    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
//    initData.pSysMem = cubeIndices;
//    device->CreateBuffer(&bd, &initData, cubeIndexBuffer.GetAddressOf());
//
//    bd.ByteWidth = sizeof(planeIndicies);
//    initData.pSysMem = planeIndicies;
//    device->CreateBuffer(&bd, &initData, planeIndexBuffer.GetAddressOf());
//
//    bd.ByteWidth = sizeof(PerModelBuffer);
//    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//    device->CreateBuffer(&bd, nullptr, perModelBuffer.GetAddressOf());
//
//    bd.ByteWidth = sizeof(PerFrameBuffer);
//    device->CreateBuffer(&bd, nullptr, perFrameBuffer.GetAddressOf());
//
//    CreateWICTextureFromFile(device.Get(), LR"(C:\Users\Фвьшт\Downloads\734.jpg)", nullptr, brickTextureSrv.GetAddressOf());
//    HRESULT result = CreateWICTextureFromFile(device.Get(), LR"(C:\Users\Фвьшт\Pictures\photo_2023-11-09_10-59-21.jpg)", nullptr, dimaTextureSrv.GetAddressOf());
//
//    D3D11_SAMPLER_DESC sampDesc = { };
//    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
//    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
//    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
//    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
//    device->CreateSamplerState(&sampDesc, sampler.GetAddressOf());
//
//    D3D11_RASTERIZER_DESC rasterDesc = { };
//    rasterDesc.FillMode = D3D11_FILL_SOLID;
//    rasterDesc.CullMode = D3D11_CULL_NONE;
//    device->CreateRasterizerState(&rasterDesc, rasterizerState.GetAddressOf());
//    deviceContext->RSSetState(rasterizerState.Get());
//}
//
//static void Render()
//{
//    constexpr float COLOR[] = { 0.0f, 0.0f, 0.1f, 1.0f };
//    deviceContext->ClearRenderTargetView(renderTarget.Get(), COLOR);
//    deviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
//
//    XMMATRIX cube1World = XMMatrixTranslation(2.0f, 2.0f, 0.0f);
//    XMMATRIX cube2World = XMMatrixTranslation(-2.0f, 2.0f, 0.0f);
//    XMMATRIX dimaWorld = XMMatrixScalingFromVector(dimaScale) * XMMatrixRotationQuaternion(dimaRotation);
//
//    XMMATRIX view = XMMatrixLookAtLH(cameraPos, XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
//    XMMATRIX proj = XMMatrixPerspectiveFovLH(XMConvertToRadians(cameraFovAngle), (float)ScreenWidth / (float)ScreenHeight, 0.01f, 100.0f);
//
//    PerModelBuffer pmb = { };
//    PerFrameBuffer pfb = { };
//
//    pmb.ModelViewProjection = XMMatrixTranspose(cube1World * view * proj);
//    deviceContext->UpdateSubresource(perModelBuffer.Get(), 0, nullptr, &pmb, 0, 0);
//
//    pfb.Time = Time::GetTotalTime();
//    pfb.DeltaTime = Time::GetDeltaTime();
//    pfb.UseWave = useWave;
//    pfb.BendAmount = bendAmount;
//    deviceContext->UpdateSubresource(perFrameBuffer.Get(), 0, nullptr, &pfb, 0, 0);
//
//    deviceContext->VSSetShader(cubeVertexShader.Get(), nullptr, 0);
//    deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);
//
//    UINT stride = sizeof(Vertex), offset = 0;
//    deviceContext->IASetVertexBuffers(0, 1, cubeVertexBuffer.GetAddressOf(), &stride, &offset);
//    deviceContext->IASetIndexBuffer(cubeIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
//    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//    deviceContext->VSSetConstantBuffers(0, 1, perModelBuffer.GetAddressOf());
//    deviceContext->VSSetConstantBuffers(1, 1, perFrameBuffer.GetAddressOf());
//    deviceContext->PSSetShaderResources(0, 1, brickTextureSrv.GetAddressOf());
//    deviceContext->PSSetSamplers(0, 1, sampler.GetAddressOf());
//
//    deviceContext->DrawIndexed(sizeof(cubeIndices) / sizeof(uint16_t), 0, 0);
//
//    pmb.ModelViewProjection = XMMatrixTranspose(cube2World * view * proj);
//    deviceContext->UpdateSubresource(perModelBuffer.Get(), 0, nullptr, &pmb, 0, 0);
//
//    deviceContext->DrawIndexed(sizeof(cubeIndices) / sizeof(uint16_t), 0, 0);
//
//    deviceContext->IASetVertexBuffers(0, 1, planeVertexBuffer.GetAddressOf(), &stride, &offset);
//    deviceContext->IASetIndexBuffer(planeIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
//
//    deviceContext->VSSetShader(planeVertexShader.Get(), nullptr, 0);
//    deviceContext->PSSetShaderResources(0, 1, dimaTextureSrv.GetAddressOf());
//    pmb.ModelViewProjection = XMMatrixTranspose(dimaWorld * view * proj);
//    deviceContext->UpdateSubresource(perModelBuffer.Get(), 0, nullptr, &pmb, 0, 0);
//
//    deviceContext->DrawIndexed(sizeof(planeIndicies) / sizeof(uint16_t), 0, 0);
//
//    swapchain->Present(1, 0);
//}
//
//static void UpdateTime()
//{
//    Time::Update();
//
//    OutputDebugString((std::to_wstring(Time::GetTotalTime()) + L'\n').c_str());
//}
//
//static void HandleKeyboardInput(WPARAM wParam)
//{
//    if (wParam == VK_ESCAPE)
//    {
//        DestroyWindow(GetActiveWindow());
//    }
//    else if (wParam == 'C')
//    {
//        isCursorVisible = !isCursorVisible;
//        ShowCursor(isCursorVisible);
//    }
//    else if (wParam == VK_ADD)
//    {
//        if (GetAsyncKeyState('X') & 0x8000)
//            dimaScale = XMVectorAdd(dimaScale, XMVectorSet(0.1f, 0.0f, 0.0f, 0.0f));
//        else if (GetAsyncKeyState('Y') & 0x8000)
//            dimaScale = XMVectorAdd(dimaScale, XMVectorSet(0.0f, 0.1f, 0.0f, 0.0f));
//        else if (GetAsyncKeyState('Z') & 0x8000)
//            dimaScale = XMVectorAdd(dimaScale, XMVectorSet(0.0f, 0.0f, 0.1f, 0.0f));
//    }
//    else if (wParam == VK_SUBTRACT)
//    {
//        if (GetAsyncKeyState('X') & 0x8000)
//            dimaScale = XMVectorSubtract(dimaScale, XMVectorSet(0.1f, 0.0f, 0.0f, 0.0f));
//        else if (GetAsyncKeyState('Y') & 0x8000)
//            dimaScale = XMVectorSubtract(dimaScale, XMVectorSet(0.0f, 0.1f, 0.0f, 0.0f));
//        else if (GetAsyncKeyState('Z') & 0x8000)
//            dimaScale = XMVectorSubtract(dimaScale, XMVectorSet(0.0f, 0.0f, 0.1f, 0.0f));
//    }
//    else if (wParam == VK_OEM_PLUS)
//    {
//        if (GetAsyncKeyState('X') & 0x8000)
//        {
//            XMVECTOR deltaRot = XMQuaternionRotationAxis(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), 0.05f);
//            dimaRotation = XMQuaternionMultiply(deltaRot, dimaRotation);
//        }
//        else if (GetAsyncKeyState('Y') & 0x8000)
//        {
//            XMVECTOR deltaRot = XMQuaternionRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), 0.05f);
//            dimaRotation = XMQuaternionMultiply(deltaRot, dimaRotation);
//        }
//        else if (GetAsyncKeyState('Z') & 0x8000)
//        {
//            XMVECTOR deltaRot = XMQuaternionRotationAxis(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), 0.05f);
//            dimaRotation = XMQuaternionMultiply(deltaRot, dimaRotation);
//        }
//
//        dimaRotation = XMQuaternionNormalize(dimaRotation);
//    }
//    else if (wParam == VK_OEM_MINUS)
//    {
//        if (GetAsyncKeyState('X') & 0x8000)
//        {
//            XMVECTOR deltaRot = XMQuaternionRotationAxis(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), 0.05f);
//            deltaRot = XMQuaternionInverse(deltaRot);
//            dimaRotation = XMQuaternionMultiply(deltaRot, dimaRotation);
//        }
//        else if (GetAsyncKeyState('Y') & 0x8000)
//        {
//            XMVECTOR deltaRot = XMQuaternionRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), 0.05f);
//            deltaRot = XMQuaternionInverse(deltaRot);
//            dimaRotation = XMQuaternionMultiply(deltaRot, dimaRotation);
//        }
//        else if (GetAsyncKeyState('Z') & 0x8000)
//        {
//            XMVECTOR deltaRot = XMQuaternionRotationAxis(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), 0.05f);
//            deltaRot = XMQuaternionInverse(deltaRot);
//            dimaRotation = XMQuaternionMultiply(deltaRot, dimaRotation);
//        }
//
//        dimaRotation = XMQuaternionNormalize(dimaRotation);
//    }
//    else if (wParam == 'W')
//    {
//        useWave = !useWave;
//    }
//}
//
//static LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//    switch (message)
//    {
//        /*case WM_KEYDOWN:
//            if (wParam == VK_NUMLOCK)
//                OutputDebugString(L"Ура");
//            return 0;*/
//    case WM_INPUT:
//    {
//        UINT size = 0;
//        GetRawInputData((HRAWINPUT)lParam, RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER));
//
//        BYTE buffer[sizeof(RAWINPUT)] = { };
//        if (size > sizeof(buffer))
//            return 0;
//
//        GetRawInputData((HRAWINPUT)lParam, RID_INPUT, buffer, &size, sizeof(RAWINPUTHEADER));
//        RAWINPUT* raw = (RAWINPUT*)buffer;
//
//        if (raw->header.dwType == RIM_TYPEMOUSE)
//        {
//            const RAWMOUSE& mouse = raw->data.mouse;
//
//            if (mouse.usButtonFlags != 0)
//                OutputDebugStringA(std::format("mouse button flags = {:#016b}\n", mouse.usButtonFlags).c_str());
//
//            if (mouse.usButtonFlags & RI_MOUSE_WHEEL)
//            {
//                SHORT wheelDelta = (SHORT)mouse.usButtonData;
//
//                constexpr float ZOOM_SPEED = 0.05f;
//
//                cameraFovAngle -= wheelDelta * ZOOM_SPEED;
//
//                if (cameraFovAngle < 20.0f)
//                    cameraFovAngle = 20.0f;
//
//                if (cameraFovAngle > 120.0f)
//                    cameraFovAngle = 120.0f;
//            }
//        }
//        else if (raw->header.dwType == RIM_TYPEKEYBOARD)
//        {
//            const RAWKEYBOARD& keyboard = raw->data.keyboard;
//            WORD scanCode = 0;
//            BOOL keyUp = keyboard.Flags & RI_KEY_BREAK;
//
//            // Ignore key overrun state and keys not mapped to any virtual key code
//            if (keyboard.MakeCode == KEYBOARD_OVERRUN_MAKE_CODE || keyboard.VKey >= UCHAR_MAX)
//                return 0;
//
//            if (keyboard.MakeCode)
//            {
//                // Compose the full scan code value with its extended byte
//                scanCode = MAKEWORD(keyboard.MakeCode & 0x7f, ((keyboard.Flags & RI_KEY_E0) ? 0xe0 : ((keyboard.Flags & RI_KEY_E1) ? 0xe1 : 0x00)));
//            }
//            else
//            {
//                // Scan code value may be empty for some buttons (for example multimedia buttons)
//                // Try to get the scan code from the virtual key code
//                scanCode = LOWORD(MapVirtualKey(keyboard.VKey, MAPVK_VK_TO_VSC_EX));
//            }
//
//            // Get the key name for debug output
//            TCHAR keyNameBuffer[MAX_PATH] = { };
//            GetKeyNameText((LONG)MAKELPARAM(0, (HIBYTE(scanCode) ? KF_EXTENDED : 0x00) | LOBYTE(scanCode)), keyNameBuffer, MAX_PATH);
//
//            // Debug output
//            TCHAR printBuffer[MAX_PATH] = { };
//            StringCchPrintf(printBuffer, MAX_PATH, TEXT("Keyboard: scanCode=%X virtualKeyCode=%X keyName=%s\r\n"), scanCode, keyboard.VKey, keyNameBuffer);
//            OutputDebugString(printBuffer);
//        }
//
//        return 0;
//    }
//    case WM_SETCURSOR:
//        if (LOWORD(lParam) == HTCLIENT)
//        {
//            SetCursor(hCursor);
//            return true;
//        }
//
//        return false;
//    case WM_DESTROY:
//        if (swapchain != nullptr)
//            swapchain->SetFullscreenState(false, nullptr);
//        PostQuitMessage(0);
//
//        return 0;
//    default:
//        return DefWindowProc(hwnd, message, wParam, lParam);
//    }
//}
//
//static int APIENTRY MywWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int nCmdShow)
//{
//    //if (FULLSCREEN)
//    //{
//    //    ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
//    //    ScreenHeight = GetSystemMetrics(SM_CYSCREEN);
//    //}
//    //else
//    //{
//    //    ScreenWidth = 800;
//    //    ScreenHeight = 600;
//    //}
//
//    //WNDCLASSEXW winClass = { };
//    //winClass.cbSize = sizeof(WNDCLASSEXW);
//    //winClass.style = CS_HREDRAW | CS_VREDRAW;
//    //winClass.lpfnWndProc = WindowProc;
//    //winClass.hInstance = hInstance;
//    //winClass.hIcon = LoadIcon(0, IDI_APPLICATION);
//    //winClass.hCursor = LoadCursor(0, IDC_ARROW);
//    //winClass.lpszClassName = L"D3D11Class";
//    //winClass.hIconSm = LoadIconW(0, IDI_APPLICATION);
//    //RegisterClassEx(&winClass);
//
//    //HWND hwnd = CreateWindowEx(WS_EX_APPWINDOW, L"D3D11Class", L"Full Screen Cube", WS_OVERLAPPEDWINDOW,
//    //    CW_USEDEFAULT, CW_USEDEFAULT, ScreenWidth, ScreenHeight, nullptr, nullptr, hInstance, nullptr);
//
//    //ShowWindow(hwnd, nCmdShow);
//
//    //RAWINPUTDEVICE rids[] =
//    //{
//    //    {
//    //        .usUsagePage = 0x01,
//    //        .usUsage = 0x02,
//    //        .dwFlags = 0,
//    //        .hwndTarget = hwnd
//    //    },
//    //    {
//    //        .usUsagePage = 0x01,
//    //        .usUsage = 0x06,
//    //        .dwFlags = 0,
//    //        .hwndTarget = hwnd
//    //    }
//    //};
//
//    //RegisterRawInputDevices(rids, 2, sizeof(RAWINPUTDEVICE));
//
//    //hCursor = LoadCursor(nullptr, IDC_ARROW);
//
//    ///*InitD3D(hwnd);*/
//
//    //MSG msg = { 0 };
//    //while (msg.message != WM_QUIT)
//    //{
//    //    if (GetMessage(&msg, nullptr, 0, 0) > 0)
//    //    {
//    //        TranslateMessage(&msg);
//    //        DispatchMessage(&msg);
//    //    }
//    //}
//
//    using namespace ByteEngine::Math;
//    using namespace ByteEngine::Math::Math;
//
//    Application app;
//    Vector3 a(10.49794f);
//    Quaternion q = Quaternion::FromEuler(90.0f, 0.0f, 0.0f);
//    Log(a.y);
//
//    return 0;
//}