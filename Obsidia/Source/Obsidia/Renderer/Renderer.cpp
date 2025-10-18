#include "obpch.h"
#include "Renderer.hpp"

#include "Obsidia/Core/Core.hpp"
#include "Obsidia/Core/Logger.hpp"

#include <Obsidian/Core/Information.hpp>

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

        , m_CompositePass(m_Device,  Obsidian::RenderpassSpecification()
            .SetBindpoint(Obsidian::PipelineBindpoint::Graphics)

            // TODO: Fix, // Note: Currently previous renderpasses already render to 
            .SetColourImageSpecification(m_SwapChain.GetImage(0).GetSpecification())
            .SetColourLoadOperation(Obsidian::LoadOperation::Load)
            .SetColourStartState(Obsidian::ResourceState::RenderTarget)
            .SetColourRenderingState(Obsidian::ResourceState::RenderTarget)
            .SetColourEndState(Obsidian::ResourceState::Present)
            
            .SetDebugName("Composite Pass")
        )
    {
        s_Instance = this;

        // Create CommandListPools & ComposeLists for each frame
        for (uint8_t i = 0; i < m_GraphicsListPools.size(); i++)
        {
            m_GraphicsListPools[i].Construct(m_SwapChain, Obsidian::CommandListPoolSpecification()
                .SetQueue(Obsidian::CommandQueue::Graphics)
                .SetDebugName("CommandPool for Graphics CommandLists")
            );

            m_CompositeLists[i].Construct(m_GraphicsListPools[i].Get(), Obsidian::CommandListSpecification()
                .SetDebugName("CommandList for CompositePass")
            );
        }
 
        // Create framebuffers for composite pass
        for (uint8_t i = 0; i < Obsidian::Information::FramesInFlight; i++)
            m_CompositePass.CreateFramebuffer(Obsidian::FramebufferSpecification()
                .SetColourAttachment(Obsidian::FramebufferAttachment()
                    .SetImage(m_SwapChain.GetImage(i))
                    .SetSubresources(Obsidian::ImageSubresourceSpecification()
                        .SetBaseArraySlice(0)
                        .SetNumArraySlices(1)
                        .SetBaseMipLevel(0)
                        .SetNumMipLevels(1)
                    )
                )
            );
    }

    Renderer::~Renderer()
    {
        m_Device.Wait();
        m_Device.DestroySwapchain(m_SwapChain);

        DestroyQueue();
        s_Instance = nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Methods
    ////////////////////////////////////////////////////////////////////////////////////
    void Renderer::Begin()
    {
        s_Instance->m_SwapChain.AcquireNextImage();
        s_Instance->m_GraphicsListPools[GetCurrentFrame()]->Reset();
    }

    void Renderer::End()
    {
        // Composite pass
        {
            auto& list = s_Instance->m_CompositeLists[GetCurrentFrame()].Get();
            list.Open();

            list.StartRenderpass(Obsidian::RenderpassStartArgs()
                .SetRenderpass(s_Instance->m_CompositePass)

                .SetViewport(Obsidian::Viewport(static_cast<float>(s_Instance->m_Window.GetSize().x), static_cast<float>(s_Instance->m_Window.GetSize().y)))
                .SetScissor(Obsidian::ScissorRect(Obsidian::Viewport(static_cast<float>(s_Instance->m_Window.GetSize().x), static_cast<float>(s_Instance->m_Window.GetSize().y))))

                .SetColourClear({ 0.0f, 0.0f, 0.0f, 1.0f })
            );

            // Note: We currently don't composite any effects

            list.EndRenderpass(Obsidian::RenderpassEndArgs()
                .SetRenderpass(s_Instance->m_CompositePass)
            );

            list.Close();
            list.Submit(Obsidian::CommandListSubmitArgs()
                .SetWaitOnLists({ s_Instance->m_LastVisualLayerList }) // Note: It should always have a value at this point
                .SetWaitForSwapchainImage(false) // Note: VisualLayer wait on the image
                .SetOnFinishMakeSwapchainPresentable(true)
            );
        }

        s_Instance->m_SwapChain.Present();
        s_Instance->DestroyQueue();

        // Reset VisualLayer waiting
        s_Instance->m_LastVisualLayerList = nullptr;
    }

    void Renderer::BeginLayer(Project::VisualLayer& layer)
    {
        layer.Begin();
    }

    void Renderer::EndLayer(Project::VisualLayer& layer)
    {
        if (s_Instance->m_LastVisualLayerList)
        {
            std::vector<const Obsidian::CommandList*> waitOn{ s_Instance->m_LastVisualLayerList };
            layer.End(waitOn);
        }
        else
            layer.End({});

        s_Instance->m_LastVisualLayerList = &layer.GetCommandList(GetCurrentFrame());
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Other
    ////////////////////////////////////////////////////////////////////////////////////
    void Renderer::Resize(uint32_t width, uint32_t height)
    {
        s_Instance->m_SwapChain.Resize(width, height);

        // TODO: Resize visuallayers
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
            // Note: We don't want to get the Obsidian Trace messages
            break;
        case Obsidian::DeviceMessageType::Info:
            Logger::Info("(OBSIDIAN) - {0}", message);
            break;
        case Obsidian::DeviceMessageType::Warn:
            Logger::Warning("(OBSIDIAN) - {0}", message);
            break;
        case Obsidian::DeviceMessageType::Error:
            Logger::Error("(OBSIDIAN) - {0}", message);
            break;

        default:
            break;
        }
    }

}