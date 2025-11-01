#include "rppch.h"
#include "Scene3DRenderer.hpp"

#include "Rapid/Core/Core.hpp"
#include "Rapid/Core/Logger.hpp"

#include "Rapid/Renderer/Renderer.hpp"

#include "Rapid/Project/Scene/Scene3D.hpp"

namespace Rapid::Project
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Constructor & Destructor
    ////////////////////////////////////////////////////////////////////////////////////
    Scene3DRenderer::Scene3DRenderer(Rapid::Renderer& internalRenderer, std::span<Obsidian::Image*, Obsidian::Information::FramesInFlight> images)
        : m_InternalRenderer(internalRenderer), m_Renderpass(internalRenderer.GetDevice(), Obsidian::RenderpassSpecification()
            .SetBindpoint(Obsidian::PipelineBindpoint::Graphics)

            .SetColourImageSpecification(images[0]->GetSpecification())
            .SetColourLoadOperation(Obsidian::LoadOperation::Clear)
            .SetColourStoreOperation(Obsidian::StoreOperation::Store)
            .SetColourStartState(Obsidian::ResourceState::Present) // TODO: Change to something
            .SetColourRenderingState(Obsidian::ResourceState::RenderTarget)
            .SetColourEndState(Obsidian::ResourceState::RenderTarget)

            .SetDebugName("Scene3DPass")
        ), m_Width(images[0]->GetSpecification().Width), m_Height(images[0]->GetSpecification().Height)
    {
        // Create CommandLists for each frame
        for (uint8_t i = 0; i < m_CommandLists.size(); i++)
            m_CommandLists[i].Construct(internalRenderer.GetGraphicsPool(i), Obsidian::CommandListSpecification()
                .SetDebugName(std::format("CommandList({0}) for Scene3DPass", i))
            );

        // Create framebuffers for renderpass
        for (uint8_t i = 0; i < Obsidian::Information::FramesInFlight; i++)
            m_Renderpass.CreateFramebuffer(Obsidian::FramebufferSpecification()
                .SetColourAttachment(Obsidian::FramebufferAttachment()
                    .SetImage(*images[i])
                )
            );
    }

    Scene3DRenderer::~Scene3DRenderer()
    {
        m_InternalRenderer.GetDevice().DestroyRenderpass(m_Renderpass);

        for (uint8_t i = 0; i < m_CommandLists.size(); i++)
            m_InternalRenderer.GetGraphicsPool(i).FreeList(m_CommandLists[i].Get());
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Methods
    ////////////////////////////////////////////////////////////////////////////////////
    void Scene3DRenderer::Render(const Scene3D& scene, bool waitForSwapchain, bool onFinishMakeSwapchainPresentable)
    {
        Begin();
        RenderScene(scene);
        End(waitForSwapchain, onFinishMakeSwapchainPresentable);
    }

    void Scene3DRenderer::Resize(uint32_t width, uint32_t height)
    {
        m_Renderpass.ResizeFramebuffers();

        m_Width = width;
        m_Height = height;
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Private methods
    ////////////////////////////////////////////////////////////////////////////////////
    void Scene3DRenderer::Begin()
    {
        auto& list = m_CommandLists[m_InternalRenderer.GetCurrentFrame()].Get();
        list.Open();

        list.StartRenderpass(Obsidian::RenderpassStartArgs()
            .SetRenderpass(m_Renderpass)

            .SetViewport(Obsidian::Viewport(static_cast<float>(m_Width), static_cast<float>(m_Height)))
            .SetScissor(Obsidian::ScissorRect(Obsidian::Viewport(static_cast<float>(m_Width), static_cast<float>(m_Height))))

            .SetColourClear({ 0.0f, 0.0f, 0.0f, 1.0f })
        );
    }

    void Scene3DRenderer::RenderScene(const Scene3D& scene)
    {
        scene.OnRender(*this);
    }

    void Scene3DRenderer::End(bool waitForSwapchain, bool onFinishMakeSwapchainPresentable)
    {
        auto& list = m_CommandLists[m_InternalRenderer.GetCurrentFrame()].Get();

        list.EndRenderpass(Obsidian::RenderpassEndArgs()
            .SetRenderpass(m_Renderpass)
        );

        list.Close();
        list.Submit(Obsidian::CommandListSubmitArgs()
            .SetWaitForSwapchainImage(waitForSwapchain)
            .SetOnFinishMakeSwapchainPresentable(onFinishMakeSwapchainPresentable)
        );
    }

}