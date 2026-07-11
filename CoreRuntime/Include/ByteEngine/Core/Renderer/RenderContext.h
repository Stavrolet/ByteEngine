#pragma once

#include <wil/com.h>

#include "ByteEngine/Core/Base/Singleton.h"
#include "ByteEngine/Math/Vector2.h"
#include "ByteEngine/Primitives.h"

struct ID3D11Device1;
struct ID3D11DeviceContext1;
struct IDXGISwapChain1;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;

namespace ByteEngine
{
    enum class WindowMode : uint8;

    class RenderingContext : public Singleton<RenderingContext>
    {
        friend class Application;

    private:
        wil::com_ptr_nothrow<ID3D11Device1> device;
        wil::com_ptr_nothrow<ID3D11DeviceContext1> deviceContext;

        wil::com_ptr_nothrow<IDXGISwapChain1> swapChain;
        wil::com_ptr_nothrow<ID3D11RenderTargetView> renderTargetView;
        wil::com_ptr_nothrow<ID3D11DepthStencilView> depthStencilView;

    public:
        RenderingContext() = default;
        virtual ~RenderingContext() override;

    private:
        void Initialize();

        void CreateDeviceAndContext();
        void CreateDebugLayer();
        void CreateSwapChain();
        bool CreateRenderTargetAndDepthStencil();

        void Update(const float* backgroundColor);
        void OnWindowResized(ByteEngine::Math::Vector2I size);
        void OnWindowModeChanged(WindowMode mode);

        void ResizeSwapchain(ByteEngine::Math::Vector2I size);
        void SetSwapChainFullscreenState(bool fullscreen);

        void Reinitialize()
        {
            Cleanup();
            Initialize();
        }

        void Cleanup();
    };
}