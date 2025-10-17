#include "obpch.h"
#include "Renderer.hpp"

#include "Obsidia/Core/Core.hpp"
#include "Obsidia/Core/Logger.hpp"

namespace Ob
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Constructor & Destructor
    ////////////////////////////////////////////////////////////////////////////////////
    Renderer::Renderer(Obsidian::Window& window, bool vsync)
        : m_Window(window)
        , m_Device(Obsidian::DeviceSpecification()
            .SetNativeWindow(m_Window.GetNativeWindow())
            .SetDestroyCallback([this](Obsidian::DeviceDestroyFn fn) { DestroyCallback(fn); })
            .SetMessageCallback([this](Obsidian::DeviceMessageType type, const std::string& message) { MessageCallback(type, message); })
        )
        , m_SwapChain(m_Device, Obsidian::SwapchainSpecification()
            .SetWindow(m_Window)
            .SetFormat(Obsidian::Format::BGRA8Unorm)
            .SetColourSpace(Obsidian::ColourSpace::SRGB)
            .SetVSync(vsync)
            .SetDebugName("SwapChain")
        )
    {
    }

    Renderer::~Renderer()
    {
        m_Device.Wait();
        m_Device.DestroySwapchain(m_SwapChain);

        DestroyQueue();
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Methods
    ////////////////////////////////////////////////////////////////////////////////////
    void Renderer::Begin()
    {
        m_SwapChain.AcquireNextImage();
    }

    void Renderer::End()
    {
        m_SwapChain.Present();
        DestroyQueue();
    }

    void Renderer::BeginLayer(const Project::VisualLayer& layer)
    {

    }

    void Renderer::EndLayer(const Project::VisualLayer& layer)
    {

    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Other
    ////////////////////////////////////////////////////////////////////////////////////
    void Renderer::Resize(uint32_t width, uint32_t height)
    {
        m_SwapChain.Resize(width, height);
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Private methods
    ////////////////////////////////////////////////////////////////////////////////////
    void Renderer::DestroyQueue()
    {
        while (!m_DestroyQueue.empty())
        {
            m_DestroyQueue.front()();
            m_DestroyQueue.pop();
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Callbacks
    ////////////////////////////////////////////////////////////////////////////////////
    void Renderer::DestroyCallback(Obsidian::DeviceDestroyFn fn)
    {
        m_DestroyQueue.push(fn);
    }

    void Renderer::MessageCallback(Obsidian::DeviceMessageType type, const std::string& message) const
    {
        switch (type)
        {
        case Obsidian::DeviceMessageType::Trace:
            Logger::Trace("{{OBSIDIAN}} - {0}", message);
            break;
        case Obsidian::DeviceMessageType::Info:
            Logger::Info("{{OBSIDIAN}} - {0}", message);
            break;
        case Obsidian::DeviceMessageType::Warn:
            Logger::Warning("{{OBSIDIAN}} - {0}", message);
            break;
        case Obsidian::DeviceMessageType::Error:
            Logger::Error("{{OBSIDIAN}} - {0}", message);
            break;

        default:
            break;
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Static getters
    ////////////////////////////////////////////////////////////////////////////////////
    Project::Scene2DRenderer& Renderer::Get2DRenderer()
    {
        static Project::Scene2DRenderer renderer;
        return renderer;
    }

    Project::Scene3DRenderer& Renderer::Get3DRenderer()
    {
        static Project::Scene3DRenderer renderer;
        return renderer;
    }

}