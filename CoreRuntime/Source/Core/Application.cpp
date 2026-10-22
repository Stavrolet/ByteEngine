#define WIN32_LEAN_AND_MEAN

#include "ByteEngine/Core/Application.h"

#include "ByteEngine/Core/Input/Input.h"
#include "ByteEngine/Core/MainWindow.h"
#include "ByteEngine/Core/Memory/Allocator.h"
#include "ByteEngine/Debug.h"
#include "ByteEngine/String/StringName.h"
#include "Platform/Core/Graphics/GraphicsDeviceD3D11.h"

#include <d3d11_1.h>

using namespace ByteEngine::Graphics;
using namespace ByteEngine::Memory;

namespace ByteEngine
{
    void Application::Quit(int32 exitCode)
    {
        this->exitCode = exitCode;
        isRunning = false;
    }

    Application::Error Application::Initialize()
    {
        StringName::InitStringPool();

        SetInstance(this);

        Debug* debug = static_cast<Debug*>(std::malloc(sizeof(Debug)));
        new (debug) Debug();
        debug->Initialize();
        Debug::SetInstance(debug);

        GraphicsDeviceD3D11* graphicsDeviceD3D11 = static_cast<GraphicsDeviceD3D11*>(std::malloc(sizeof(GraphicsDeviceD3D11)));
        new (graphicsDeviceD3D11) GraphicsDeviceD3D11();

#ifdef BE_DEBUG
        GraphicsDevice::Error error = graphicsDeviceD3D11->Initialize(true);
#else
        GraphicsDevice::Error error = graphicsDeviceD3D11->Initialize(false);
#endif

        if (error != GraphicsDevice::Error::Success)
        {
            BE_LOG_ERROR("Failed to initialize graphics device. Error code: {}", static_cast<int>(error));
            CleanupSingletons();
            return static_cast<Error>(error);
        }

        GraphicsDevice::SetInstance(graphicsDeviceD3D11);

        MainWindow::GetInstance().Initialize("ByteEngine");

        Input* input = static_cast<Input*>(std::malloc(sizeof(Input)));
        new (input) Input();
        Input::SetInstance(input);

        return Error::Success;
    }

    int32 Application::Run()
    {
        MainWindow& mainWindow = MainWindow::GetInstance();
        Input& input = Input::GetInstance();

        while (isRunning)
        {
            mainWindow.PollEvents();

            if (mainWindow.closeRequested)
            {
                if (quitRequest.HasSubscriber())
                {
                    if (quitRequest.Invoke())
                    {
                        BE_LOG_INFO("Quit request was approved. Closing application.");
                        mainWindow.Close();
                        break;
                    }
                    else
                    {
                        BE_LOG_INFO("Quit request was denied. Continuing application execution.");
                        mainWindow.closeRequested = false;
                    }
                }
                else
                {
                    BE_LOG_INFO("Quit request was approved. Closing application.");
                    mainWindow.Close();
                    break;
                }
            }

            input.Update();
        }

        BE_LOG_INFO("Application is closing");

        CleanupSingletons();
        return exitCode;
    }

    void Application::FatalCrashImpl(std::string_view msg) const
    {
        BE_LOG_CRITICAL("{}", msg);

        if (Debug::IsDebuggerAttached())
            Debug::Breakpoint();
        else
            std::abort();
    }

    void Application::CleanupSingletons() const
    {
        std::free(&Input::GetInstance());
        std::free(&GraphicsDevice::GetInstance());
        std::free(&Debug::GetInstance());
        std::free(&Allocator::GetInstance());
    }
} // namespace ByteEngine