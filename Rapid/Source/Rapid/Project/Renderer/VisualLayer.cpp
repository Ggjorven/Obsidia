#include "rppch.h"
#include "VisualLayer.hpp"

#include "Rapid/Core/Core.hpp"
#include "Rapid/Core/Logger.hpp"

#include "Rapid/Project/Renderer/Renderer.hpp"

#include "Rapid/Renderer/Renderer.hpp"

#include <ranges>

namespace Rapid::Project
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Constructor & Destructor
    ////////////////////////////////////////////////////////////////////////////////////
    VisualLayer::VisualLayer(Renderer& targetRenderer, const VisualLayerSpecification& specs)
        : m_TargetRenderer(targetRenderer), m_Specification(specs)

        , m_Renderpass(targetRenderer.GetInternalRenderer().GetDevice(), Obsidian::RenderpassSpecification()
            .SetBindpoint(Obsidian::PipelineBindpoint::Graphics)

            .SetColourImageSpecification(m_TargetRenderer.GetImage(0).GetSpecification())
            .SetColourLoadOperation(((m_Specification.Level == 1) ? Obsidian::LoadOperation::Clear : Obsidian::LoadOperation::Load))
            .SetColourStartState(Obsidian::ResourceState::Present)
            .SetColourRenderingState(Obsidian::ResourceState::RenderTarget)
            .SetColourEndState(Obsidian::ResourceState::RenderTarget)

            .SetDebugName(std::format("VisualLayer-{0} Pass", m_Specification.Level))
        )
    {
        // Create CommandLists for each frame
        for (uint8_t i = 0; i < m_CommandLists.size(); i++)
            m_CommandLists[i].Construct(m_TargetRenderer.GetInternalRenderer().GetGraphicsPool(i), Obsidian::CommandListSpecification()
                .SetDebugName(std::format("CommandList({0}) for VisualLayer-{1}", i, m_Specification.Level))
            );

        // Create framebuffers for renderpass
        for (uint8_t i = 0; i < Obsidian::Information::FramesInFlight; i++)
            m_Renderpass.CreateFramebuffer(Obsidian::FramebufferSpecification()
                .SetColourAttachment(Obsidian::FramebufferAttachment()
                    .SetImage(m_TargetRenderer.GetImage(i))
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
            m_TargetRenderer.GetInternalRenderer().GetGraphicsPool(i).FreeList(m_CommandLists[i].Get());

        m_TargetRenderer.GetInternalRenderer().GetDevice().DestroyRenderpass(m_Renderpass);
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Methods
    ////////////////////////////////////////////////////////////////////////////////////
    void VisualLayer::Begin()
    {
        auto& list = m_CommandLists[m_TargetRenderer.GetCurrentFrame()].Get();
        list.Open();

        list.StartRenderpass(Obsidian::RenderpassStartArgs()
            .SetRenderpass(m_Renderpass)

            .SetViewport(Obsidian::Viewport(static_cast<float>(m_TargetRenderer.GetWidth()), static_cast<float>(m_TargetRenderer.GetHeight())))
            .SetScissor(Obsidian::ScissorRect(Obsidian::Viewport(static_cast<float>(m_TargetRenderer.GetWidth()), static_cast<float>(m_TargetRenderer.GetHeight()))))

            .SetColourClear({ 0.0f, 0.0f, 0.0f, 1.0f })
        );
    }

    void VisualLayer::End(std::span<const Obsidian::CommandList*> waitOn)
    {
        auto& list = m_CommandLists[m_TargetRenderer.GetCurrentFrame()].Get();

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