#include <d3d11_2.h>
#include <wil/com.h>

#include "Core/Graphics/GraphicsDevice.h"

namespace ByteEngine::Graphics
{
    class GraphicsDeviceD3D11 final : public GraphicsDevice
    {
    private:
        template<typename T>
            requires std::derived_from<T, IUnknown>
        using ComPtr = wil::com_ptr_nothrow<T>;

    private:
        ComPtr<ID3D11Device2> device;

    public:
        ~GraphicsDeviceD3D11() override = default;

        Error Initialize(bool enableDebug) override;

    private:
        void CreateDebugLayer();
    };
}