#include "rppch.h"
#include "UIRenderer.hpp"

#include "Rapid/Core/Core.hpp"
#include "Rapid/Core/Logger.hpp"

#include "Rapid/Core/Window.hpp"

namespace Rapid::Project
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Constructor & Destructor
    ////////////////////////////////////////////////////////////////////////////////////
    UIRenderer::UIRenderer(Renderer& targetRenderer)
        : m_TargetRenderer(targetRenderer), m_Renderpass(targetRenderer.GetInternalRenderer().GetDevice(), Obsidian::RenderpassSpecification()
            .SetBindpoint(Obsidian::PipelineBindpoint::Graphics)

            .SetColourImageSpecification(m_TargetRenderer.GetImage(0).GetSpecification())
            .SetColourLoadOperation(Obsidian::LoadOperation::Clear)
            .SetColourStartState(Obsidian::ResourceState::RenderTarget)
            .SetColourRenderingState(Obsidian::ResourceState::RenderTarget)
            .SetColourEndState(Obsidian::ResourceState::ShaderResource)

            .SetDebugName("UIPass")
        )
    {
        // Create CommandLists for each frame
        for (uint8_t i = 0; i < m_CommandLists.size(); i++)
            m_CommandLists[i].Construct(m_TargetRenderer.GetInternalRenderer().GetGraphicsPool(i), Obsidian::CommandListSpecification()
                .SetDebugName(std::format("CommandList({0}) for UIPass", i))
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

    UIRenderer::~UIRenderer()
    {
        m_TargetRenderer.GetInternalRenderer().GetDevice().DestroyRenderpass(m_Renderpass);

        for (uint8_t i = 0; i < Obsidian::Information::FramesInFlight; i++)
            m_TargetRenderer.GetInternalRenderer().GetGraphicsPool(i).FreeList(m_CommandLists[i].Get());
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Methods
    ////////////////////////////////////////////////////////////////////////////////////
    void UIRenderer::Render(const Obsidian::CommandList& waitOn)
    {
        Begin();
        RenderUI();
        End(waitOn);
    }

    void UIRenderer::Resize()
    {
        m_Renderpass.ResizeFramebuffers();
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Private methods
    ////////////////////////////////////////////////////////////////////////////////////
    void UIRenderer::Begin()
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

    void UIRenderer::RenderUI()
    {

    }

    void UIRenderer::End(const Obsidian::CommandList& waitOn)
    {
        auto& list = m_CommandLists[m_TargetRenderer.GetCurrentFrame()].Get();

        list.EndRenderpass(Obsidian::RenderpassEndArgs()
            .SetRenderpass(m_Renderpass)
        );

        list.Close();
        list.Submit(Obsidian::CommandListSubmitArgs()
            .SetWaitOnLists({ &waitOn })
            .SetWaitForSwapchainImage(false)
            .SetOnFinishMakeSwapchainPresentable(false)
        );
    }

}