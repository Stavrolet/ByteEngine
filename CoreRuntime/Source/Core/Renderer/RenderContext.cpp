#include <d3d11_1.h>
#ifdef _DEBUG
#include <dxgidebug.h>
#include <dxgi1_3.h>
#endif
#include <Windows.h>

#include "ByteEngine/Core/Renderer/RenderContext.h"

#include "ByteEngine/Core/Base/MainWindow.h"
#include "ByteEngine/Debug.h"

using namespace ByteEngine::Math;

namespace ByteEngine
{
    RenderingContext::~RenderingContext()
    {
        Cleanup();

#ifdef _DEBUG
        wil::com_ptr_nothrow<IDXGIDebug1> debug;
        DXGIGetDebugInterface1(0, __uuidof(IDXGIDebug1), debug.put_void());
        debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
#endif
    }

    void RenderingContext::Initialize()
    {
        MainWindow::GetInstance().Resized().SubscribeRawPointer(this, &RenderingContext::OnWindowResized);
        MainWindow::GetInstance().ModeChanged().SubscribeRawPointer(this, &RenderingContext::OnWindowModeChanged);

        int32 attempts = 0;

        while (attempts < 5)
        {
            CreateDeviceAndContext();
#ifdef _DEBUG
            CreateDebugLayer();
#endif
            CreateSwapChain();

            if (CreateRenderTargetAndDepthStencil())
                break;

            attempts++;
            Cleanup();
            Sleep(80);
        }
    }

    void RenderingContext::CreateDeviceAndContext()
    {
        wil::com_ptr_nothrow<ID3D11Device> baseDevice;
        wil::com_ptr_nothrow<ID3D11DeviceContext> baseDeviceContext;

        D3D_FEATURE_LEVEL featureLevels[] =
        {
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0,
        };

        uint32 creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
        creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

        HRESULT hResult = D3D11CreateDevice(
            0, D3D_DRIVER_TYPE_HARDWARE,
            0, creationFlags,
            featureLevels, ARRAYSIZE(featureLevels),
            D3D11_SDK_VERSION, baseDevice.put(),
            0, baseDeviceContext.put()
        );

        if (FAILED(hResult))
            BE_LOG_CRITICAL("Failed to start the graphics system.\nCheck if your GPU supports DirectX 11.1 or if your GPU driver is working properly. Error code: {}", hResult);

        hResult = baseDevice->QueryInterface(__uuidof(ID3D11Device1), device.put_void());
        if (FAILED(hResult))
            BE_LOG_CRITICAL("Failed to start the graphics system.\nCheck if your GPU supports DirectX 11.1 or if your GPU driver is working properly. Error code: {}", hResult);
        hResult = baseDeviceContext->QueryInterface(__uuidof(ID3D11DeviceContext1), deviceContext.put_void());
        if (FAILED(hResult))
            BE_LOG_CRITICAL("Failed to start the graphics system.\nCheck if your GPU supports DirectX 11.1 or if your GPU driver is working properly. Error code: {}", hResult);
    }

    void RenderingContext::CreateDebugLayer()
    {
        wil::com_ptr_nothrow<ID3D11Debug> debug = nullptr;
        HRESULT hResult = device->QueryInterface(__uuidof(ID3D11Debug), debug.put_void());
        if (FAILED(hResult))
        {
            BE_LOGV_ERROR(hResult);
            return;
        }

        wil::com_ptr_nothrow<ID3D11InfoQueue> infoQueue = nullptr;
        hResult = debug->QueryInterface(__uuidof(ID3D11InfoQueue), infoQueue.put_void());
        if (FAILED(hResult))
        {
            DebugHelper::LogDebugError(hResult);
            return;
        }

        infoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
        infoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
    }

    void RenderingContext::CreateSwapChain()
    {
        wil::com_ptr_nothrow<IDXGIDevice1> dxgiDevice;
        HRESULT hResult = device->QueryInterface(__uuidof(IDXGIDevice1), dxgiDevice.put_void());
        if (FAILED(hResult))
            DebugHelper::LogDebugError(hResult);

        wil::com_ptr_nothrow<IDXGIAdapter> adapter;
        hResult = dxgiDevice->GetAdapter(adapter.put());
        if (FAILED(hResult))
            DebugHelper::LogDebugError(hResult);

        wil::com_ptr_nothrow<IDXGIAdapter1> adapter1;
        hResult = adapter->QueryInterface(__uuidof(IDXGIAdapter1), adapter1.put_void());
        if (FAILED(hResult))
            DebugHelper::LogDebugError(hResult);

        DXGI_ADAPTER_DESC1 adapterDesc;
        adapter1->GetDesc1(&adapterDesc);

        wil::com_ptr_nothrow<IDXGIFactory2> dxgiFactory;
        hResult = adapter1->GetParent(__uuidof(IDXGIFactory2), dxgiFactory.put_void());
        if (FAILED(hResult))
            DebugHelper::LogDebugError(hResult);

        wil::com_ptr_nothrow<IDXGIOutput> output;
        hResult = adapter1->EnumOutputs(0, output.put());
        if (FAILED(hResult))
            DebugHelper::LogDebugError(hResult);

        wil::com_ptr_nothrow<IDXGIOutput1> output1;
        hResult = output->QueryInterface(__uuidof(IDXGIOutput1), output1.put_void());
        if (FAILED(hResult))
            DebugHelper::LogDebugError(hResult);

        DXGI_MODE_DESC1 targetModeDesc = { };
        targetModeDesc.Width = 1920;
        targetModeDesc.Height = 1080;
        targetModeDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        targetModeDesc.RefreshRate.Numerator = 120;
        targetModeDesc.RefreshRate.Denominator = 1;

        DXGI_MODE_DESC1 closestModeDesc = { };

        hResult = output1->FindClosestMatchingMode1(&targetModeDesc, &closestModeDesc, device.get());
        if (FAILED(hResult))
            DebugHelper::LogDebugError(hResult);

        //DXGI_MODE_DESC* displayModes = nullptr;
        //uint32 numModes = 0;
        //uint32 attempts = 0;

        //while (attempts < 5)
        //{
        //    hResult = output->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, 0, &numModes, nullptr);
        //    if (FAILED(hResult))
        //        DebugHelper::LogDebugError(hResult);

        //    displayModes = new DXGI_MODE_DESC[numModes];
        //    hResult = output->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, 0, &numModes, displayModes);

        //    if (SUCCEEDED(hResult))
        //        break;

        //    attempts++;
        //    delete[] displayModes;
        //}

        //if (attempts >= 5)
        //    DebugHelper::LogCriticalError("Failed to start the graphics system.\nCheck if your GPU supports DirectX 11.1 or if your GPU driver is working properly.\n", hResult);

        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { };
        swapChainDesc.Width = 0;
        swapChainDesc.Height = 0;
        swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = 2;
        swapChainDesc.Scaling = DXGI_SCALING_NONE;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
        swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenSwapChainDesc = { };
        fullScreenSwapChainDesc.RefreshRate = closestModeDesc.RefreshRate;
        fullScreenSwapChainDesc.Windowed = true;
        fullScreenSwapChainDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        fullScreenSwapChainDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

        hResult = dxgiFactory->CreateSwapChainForHwnd(device.get(), static_cast<HWND>(MainWindow::GetInstance().GetNativeHandle()), &swapChainDesc, &fullScreenSwapChainDesc, nullptr, swapChain.put());
        if (FAILED(hResult))
            DebugHelper::LogCriticalError("Failed to start the graphics system.\nCheck if your GPU supports DirectX 11.1 or if your GPU driver is working properly.\n", hResult);

        dxgiFactory->MakeWindowAssociation(static_cast<HWND>(MainWindow::GetInstance().GetNativeHandle()), DXGI_MWA_NO_ALT_ENTER | DXGI_MWA_NO_WINDOW_CHANGES);
        if (MainWindow::GetInstance().GetMode() == WindowMode::ExclusiveFullscreen)
        {
            hResult = swapChain->SetFullscreenState(true, nullptr);
            DebugHelper::LogDebugMessage("FullScreen state changed.");

            if (FAILED(hResult))
            {
                if (hResult == DXGI_STATUS_MODE_CHANGE_IN_PROGRESS)
                {
                    Sleep(50);
                }
                else if (hResult != DXGI_ERROR_NOT_CURRENTLY_AVAILABLE)
                {
                    MessageBox(nullptr, L"Application failed to enter fullscreen mode. Try again later.", L"Error", MB_OK | MB_ICONERROR);
                    MainWindow::GetInstance().SetWindowMode(WindowMode::Mazimized);
                    DebugHelper::LogDebugMessage("FullScreen state changed.");
                }
            }

            hResult = swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

            if (FAILED(hResult))
            {
                if (hResult == DXGI_ERROR_DEVICE_REMOVED || hResult == DXGI_ERROR_DEVICE_RESET)
                {
                    HRESULT reason = device->GetDeviceRemovedReason();

                    if (reason == DXGI_ERROR_DRIVER_INTERNAL_ERROR)
                        DebugHelper::LogCriticalError("Internal driver error. Try restarting the application or restarting the computer.", reason);
                    else if (reason == DXGI_ERROR_DEVICE_REMOVED || reason == DXGI_ERROR_DEVICE_RESET)
                        Reinitialize();
                }
            }
        }
    }

    bool RenderingContext::CreateRenderTargetAndDepthStencil()
    {
        wil::com_ptr_nothrow<ID3D11Texture2D> frameBuffer;
        HRESULT hResult = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), frameBuffer.put_void());

        if (FAILED(hResult))
        {
            if (hResult == DXGI_ERROR_DEVICE_REMOVED || hResult == DXGI_ERROR_DEVICE_RESET)
            {
                HRESULT reason = device->GetDeviceRemovedReason();

                if (reason == DXGI_ERROR_DRIVER_INTERNAL_ERROR)
                    DebugHelper::LogCriticalError("Internal driver error. Try restarting the application, restarting the computer or reinstalling the GPU driver.", reason);
                else if (reason == DXGI_ERROR_DEVICE_REMOVED || reason == DXGI_ERROR_DEVICE_RESET)
                    return false;
            }
        }

        hResult = device->CreateRenderTargetView(frameBuffer.get(), nullptr, renderTargetView.put());
        if (FAILED(hResult))
            DebugHelper::LogCriticalError("Failed to start the graphics system.\nCheck if your GPU supports DirectX 11.1 or if your GPU driver is working properly.\n", hResult);

        D3D11_TEXTURE2D_DESC depthBufferDesc;
        frameBuffer->GetDesc(&depthBufferDesc);

        depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

        wil::com_ptr_nothrow<ID3D11Texture2D> depthBuffer;
        device->CreateTexture2D(&depthBufferDesc, nullptr, depthBuffer.put());
        if (FAILED(hResult))
            DebugHelper::LogCriticalError("Failed to start the graphics system.\nCheck if your GPU supports DirectX 11.1 or if your GPU driver is working properly.\n", hResult);

        hResult = device->CreateDepthStencilView(depthBuffer.get(), nullptr, depthStencilView.put());
        if (FAILED(hResult))
            DebugHelper::LogCriticalError("Failed to start the graphics system.\nCheck if your GPU supports DirectX 11.1 or if your GPU driver is working properly.\n", hResult);

        deviceContext->OMSetRenderTargets(1, renderTargetView.addressof(), depthStencilView.get());

        D3D11_VIEWPORT viewport = { };
        viewport.TopLeftX = 0.0f;
        viewport.TopLeftY = 0.0f;
        viewport.Width = static_cast<float>(MainWindow::GetInstance().GetSize().width);
        viewport.Height = static_cast<float>(MainWindow::GetInstance().GetSize().height);
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;
        deviceContext->RSSetViewports(1, &viewport);

        return true;
    }

    void RenderingContext::Update(const float backgroundColor[4])
    {
        deviceContext->ClearRenderTargetView(renderTargetView.get(), backgroundColor);
        deviceContext->ClearDepthStencilView(depthStencilView.get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

        HRESULT hResult = swapChain->Present(1, 0);
        if (FAILED(hResult))
        {
            if (hResult == DXGI_ERROR_DEVICE_REMOVED || hResult == DXGI_ERROR_DEVICE_RESET)
            {
                HRESULT reason = device->GetDeviceRemovedReason();

                if (reason == DXGI_ERROR_DRIVER_INTERNAL_ERROR)
                    DebugHelper::LogCriticalError("Internal driver error. Try restarting the application or restarting the computer.", reason);
                else if (reason == DXGI_ERROR_DEVICE_REMOVED || reason == DXGI_ERROR_DEVICE_RESET)
                    Reinitialize();
            }
            else
            {
                DebugHelper::LogDebugError(hResult);
            }
        }
    }

    void RenderingContext::OnWindowResized(Math::Vector2I size)
    {
        deviceContext->OMSetRenderTargets(0, nullptr, nullptr);
        ResizeSwapchain(size);
    }

    void RenderingContext::OnWindowModeChanged(WindowMode mode)
    {
        DebugHelper::LogDebugMessage("Window mode changed.");

        if (mode == WindowMode::ExclusiveFullscreen)
        {
            BOOL previousSwapChainFullscreenState;
            swapChain->GetFullscreenState(&previousSwapChainFullscreenState, nullptr);

            if (!static_cast<bool>(previousSwapChainFullscreenState))
            {
                SetSwapChainFullscreenState(true);
            }
        }
        else
        {
            BOOL previousSwapChainFullscreenState;
            swapChain->GetFullscreenState(&previousSwapChainFullscreenState, nullptr);

            if (static_cast<bool>(previousSwapChainFullscreenState))
                SetSwapChainFullscreenState(false);
        }

        ResizeSwapchain(ByteEngine::Math::Vector2I::Zero());
    }

    void RenderingContext::ResizeSwapchain(ByteEngine::Math::Vector2I size)
    {
        renderTargetView.reset();
        depthStencilView.reset();

        HRESULT hResult = swapChain->ResizeBuffers(0, static_cast<UINT>(size.x), static_cast<UINT>(size.y), DXGI_FORMAT_UNKNOWN, 0);

        if (FAILED(hResult))
        {
            if (hResult == DXGI_ERROR_DEVICE_REMOVED || hResult == DXGI_ERROR_DEVICE_RESET)
            {
                HRESULT reason = device->GetDeviceRemovedReason();

                if (reason == DXGI_ERROR_DRIVER_INTERNAL_ERROR)
                {
                    DebugHelper::LogCriticalError("Internal driver error. Try restarting the application or restarting the computer.", reason);
                }
                else if (reason == DXGI_ERROR_DEVICE_REMOVED || reason == DXGI_ERROR_DEVICE_RESET)
                {
                    Reinitialize();
                    return;
                }
            }
        }

        if (!CreateRenderTargetAndDepthStencil())
            Reinitialize();
    }

    void RenderingContext::SetSwapChainFullscreenState(bool fullscreen)
    {
        HRESULT hResult = swapChain->SetFullscreenState(fullscreen, nullptr);

        if (FAILED(hResult))
        {
            if (hResult == DXGI_STATUS_MODE_CHANGE_IN_PROGRESS)
            {
                Sleep(70);
            }
            else if (hResult != DXGI_ERROR_NOT_CURRENTLY_AVAILABLE)
            {
                DebugHelper::LogDebugError(hResult);
                MessageBox(nullptr, L"Application failed to enter exclusive fullscreen mode. Try again later.", L"Error", MB_OK | MB_ICONERROR);
                MainWindow::GetInstance().SetWindowMode(WindowMode::Mazimized);
            }
        }
    }

    void RenderingContext::Cleanup()
    {
        device.reset();
        deviceContext.reset();

        swapChain.reset();
        renderTargetView.reset();
        depthStencilView.reset();
    }
}