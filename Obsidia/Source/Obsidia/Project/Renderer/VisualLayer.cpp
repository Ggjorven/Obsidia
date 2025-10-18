#include "obpch.h"
#include "VisualLayer.hpp"

#include "Obsidia/Core/Core.hpp"
#include "Obsidia/Core/Logger.hpp"

#include "Obsidia/Renderer/Renderer.hpp"

#include <ranges>

namespace Ob::Project
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Constructor & Destructor
    ////////////////////////////////////////////////////////////////////////////////////
    VisualLayer::VisualLayer(const VisualLayerSpecification& specs)
        : m_Specification(specs)

        , m_Renderpass(Renderer::GetDevice(), Obsidian::RenderpassSpecification()
            .SetBindpoint(Obsidian::PipelineBindpoint::Graphics)

            .SetColourImageSpecification(Renderer::GetSwapChain().GetImage(0).GetSpecification())
            .SetColourLoadOperation(Obsidian::LoadOperation::Load)
            .SetColourStartState(Obsidian::ResourceState::Present)
            .SetColourRenderingState(Obsidian::ResourceState::RenderTarget)
            .SetColourEndState(Obsidian::ResourceState::RenderTarget)

            .SetDebugName(std::format("VisualLayer-{0} Pass", m_Specification.Level))
        )
    {
        // Create CommandLists for each frame
        for (uint8_t i = 0; i < m_CommandLists.size(); i++)
            m_CommandLists[i].Construct(Renderer::GetGraphicsPool(i), Obsidian::CommandListSpecification()
                .SetDebugName(std::format("CommandList({0}) for VisualLayer-{1}", i, m_Specification.Level))
            );

        // Create framebuffers for renderpass
        for (uint8_t i = 0; i < Obsidian::Information::FramesInFlight; i++)
            m_Renderpass.CreateFramebuffer(Obsidian::FramebufferSpecification()
                .SetColourAttachment(Obsidian::FramebufferAttachment()
                    .SetImage(Renderer::GetSwapChain().GetImage(i))
                    .SetSubresources(Obsidian::ImageSubresourceSpecification()
                        .SetBaseArraySlice(0)
                        .SetNumArraySlices(1)
                        .SetBaseMipLevel(0)
                        .SetNumMipLevels(1)
                    )
                )
            );
    }

    VisualLayer::~VisualLayer()
    {
        for (uint8_t i = 0; i < m_CommandLists.size(); i++)
            Renderer::GetGraphicsPool(i).FreeList(m_CommandLists[i].Get());

        Renderer::GetDevice().DestroyRenderpass(m_Renderpass);
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Methods
    ////////////////////////////////////////////////////////////////////////////////////
    void VisualLayer::Begin()
    {
        auto& list = m_CommandLists[Renderer::GetCurrentFrame()].Get();
        list.Open();

        list.StartRenderpass(Obsidian::RenderpassStartArgs()
            .SetRenderpass(m_Renderpass)

            .SetViewport(Obsidian::Viewport(static_cast<float>(Renderer::GetWindow().GetSize().x), static_cast<float>(Renderer::GetWindow().GetSize().y)))
            .SetScissor(Obsidian::ScissorRect(Obsidian::Viewport(static_cast<float>(Renderer::GetWindow().GetSize().x), static_cast<float>(Renderer::GetWindow().GetSize().y))))

            .SetColourClear({ 0.0f, 0.0f, 0.0f, 1.0f })
        );
    }

    void VisualLayer::End(std::span<const Obsidian::CommandList*> waitOn)
    {
        auto& list = m_CommandLists[Renderer::GetCurrentFrame()].Get();

        list.EndRenderpass(Obsidian::RenderpassEndArgs()
            .SetRenderpass(m_Renderpass)
        );

        list.Close();
        list.Submit(Obsidian::CommandListSubmitArgs()
            .SetWaitOnLists(waitOn)
            .SetWaitForSwapchainImage((m_Specification.Level == 1)) // Note: Only VisualLayer 1 waits on the image
            .SetOnFinishMakeSwapchainPresentable(false)
        );
    }

    void VisualLayer::Resize()
    {
        m_Renderpass.ResizeFramebuffers();
    }

}