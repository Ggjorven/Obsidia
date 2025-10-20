#include "rppch.h"
#include "Scene3DRenderer.hpp"

#include "Rapid/Core/Core.hpp"
#include "Rapid/Core/Logger.hpp"

#include "Rapid/Core/Window.hpp"

namespace Rapid::Project
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Constructor & Destructor
    ////////////////////////////////////////////////////////////////////////////////////
    Scene3DRenderer::Scene3DRenderer(Renderer& targetRenderer)
        : m_TargetRenderer(targetRenderer), m_Renderpass(m_TargetRenderer.GetInternalRenderer().GetDevice(), Obsidian::RenderpassSpecification()
            .SetBindpoint(Obsidian::PipelineBindpoint::Graphics)

            .SetColourImageSpecification(m_TargetRenderer.GetImage(0).GetSpecification())
            .SetColourLoadOperation(Obsidian::LoadOperation::Clear)
            .SetColourStartState(Obsidian::ResourceState::ShaderResource)
            .SetColourRenderingState(Obsidian::ResourceState::RenderTarget)
            .SetColourEndState(Obsidian::ResourceState::RenderTarget)

            .SetDebugName("Scene3DPass")
        )
    {
        // Create CommandLists for each frame
        for (uint8_t i = 0; i < m_CommandLists.size(); i++)
            m_CommandLists[i].Construct(m_TargetRenderer.GetInternalRenderer().GetGraphicsPool(i), Obsidian::CommandListSpecification()
                .SetDebugName(std::format("CommandList({0}) for Scene3DPass", i))
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

    Scene3DRenderer::~Scene3DRenderer()
    {
        m_TargetRenderer.GetInternalRenderer().GetDevice().DestroyRenderpass(m_Renderpass);

        for (uint8_t i = 0; i < Obsidian::Information::FramesInFlight; i++)
            m_TargetRenderer.GetInternalRenderer().GetGraphicsPool(i).FreeList(m_CommandLists[i].Get());
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Methods
    ////////////////////////////////////////////////////////////////////////////////////
    void Scene3DRenderer::Render(const Scene3D& scene)
    {
        Begin();
        RenderScene(scene);
        End();
    }

    void Scene3DRenderer::Resize()
    {
        m_Renderpass.ResizeFramebuffers();
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Private methods
    ////////////////////////////////////////////////////////////////////////////////////
    void Scene3DRenderer::Begin()
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

    void Scene3DRenderer::RenderScene(const Scene3D& scene)
    {
    }

    void Scene3DRenderer::End()
    {
        auto& list = m_CommandLists[m_TargetRenderer.GetCurrentFrame()].Get();

        list.EndRenderpass(Obsidian::RenderpassEndArgs()
            .SetRenderpass(m_Renderpass)
        );

        list.Close();
        list.Submit(Obsidian::CommandListSubmitArgs()
            .SetWaitForSwapchainImage(false)
            .SetOnFinishMakeSwapchainPresentable(false)
        );
    }

}