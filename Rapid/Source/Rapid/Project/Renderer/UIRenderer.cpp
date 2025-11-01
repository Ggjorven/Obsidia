#include "rppch.h"
#include "UIRenderer.hpp"

#include "Rapid/Core/Core.hpp"
#include "Rapid/Core/Logger.hpp"

#include "Rapid/Renderer/Renderer.hpp"

namespace Rapid::Project
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Constructor & Destructor
    ////////////////////////////////////////////////////////////////////////////////////
    UIRenderer::UIRenderer(Rapid::Renderer& internalRenderer, std::span<Obsidian::Image*, Obsidian::Information::FramesInFlight> images)
        : m_InternalRenderer(internalRenderer), m_Renderpass(m_InternalRenderer.GetDevice(), Obsidian::RenderpassSpecification()
            .SetBindpoint(Obsidian::PipelineBindpoint::Graphics)

            .SetColourImageSpecification(images[0]->GetSpecification())
            .SetColourLoadOperation(Obsidian::LoadOperation::Load)
            .SetColourStoreOperation(Obsidian::StoreOperation::Store)
            .SetColourStartState(Obsidian::ResourceState::RenderTarget)
            .SetColourRenderingState(Obsidian::ResourceState::RenderTarget)
            .SetColourEndState(Obsidian::ResourceState::Present) // TODO: Ask for it through parameter

            .SetDebugName("UIPass")
        ), m_Width(images[0]->GetSpecification().Width), m_Height(images[0]->GetSpecification().Height)
    {
        // Create CommandLists for each frame
        for (uint8_t i = 0; i < m_CommandLists.size(); i++)
            m_CommandLists[i].Construct(m_InternalRenderer.GetGraphicsPool(i), Obsidian::CommandListSpecification()
                .SetDebugName(std::format("CommandList({0}) for UIPass", i))
            );

        // Create framebuffers for renderpass
        for (uint8_t i = 0; i < Obsidian::Information::FramesInFlight; i++)
            m_Renderpass.CreateFramebuffer(Obsidian::FramebufferSpecification()
                .SetColourAttachment(Obsidian::FramebufferAttachment()
                    .SetImage(*images[0])
                )
            );
    }

    UIRenderer::~UIRenderer()
    {
        m_InternalRenderer.GetDevice().DestroyRenderpass(m_Renderpass);

        for (uint8_t i = 0; i < m_CommandLists.size(); i++)
            m_InternalRenderer.GetGraphicsPool(i).FreeList(m_CommandLists[i].Get());
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Methods
    ////////////////////////////////////////////////////////////////////////////////////
    void UIRenderer::Render(const Obsidian::CommandList& waitOn, bool waitForSwapchain, bool onFinishMakeSwapchainPresentable)
    {
        Begin();
        RenderUI();
        End(waitOn, waitForSwapchain, onFinishMakeSwapchainPresentable);
    }

    void UIRenderer::Resize(uint32_t width, uint32_t height)
    {
        m_Renderpass.ResizeFramebuffers();

        m_Width = width;
        m_Height = height;
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Private methods
    ////////////////////////////////////////////////////////////////////////////////////
    void UIRenderer::Begin()
    {
        auto& list = m_CommandLists[m_InternalRenderer.GetCurrentFrame()].Get();
        list.Open();

        list.StartRenderpass(Obsidian::RenderpassStartArgs()
            .SetRenderpass(m_Renderpass)

            .SetViewport(Obsidian::Viewport(static_cast<float>(m_Width), static_cast<float>(m_Height)))
            .SetScissor(Obsidian::ScissorRect(Obsidian::Viewport(static_cast<float>(m_Width), static_cast<float>(m_Height))))
        );
    }

    void UIRenderer::RenderUI()
    {

    }

    void UIRenderer::End(const Obsidian::CommandList& waitOn, bool waitForSwapchain, bool onFinishMakeSwapchainPresentable)
    {
        auto& list = m_CommandLists[m_InternalRenderer.GetCurrentFrame()].Get();

        list.EndRenderpass(Obsidian::RenderpassEndArgs()
            .SetRenderpass(m_Renderpass)
        );

        list.Close();
        list.Submit(Obsidian::CommandListSubmitArgs()
            .SetWaitOnLists({ &waitOn })
            .SetWaitForSwapchainImage(waitForSwapchain)
            .SetOnFinishMakeSwapchainPresentable(onFinishMakeSwapchainPresentable)
        );
    }

}