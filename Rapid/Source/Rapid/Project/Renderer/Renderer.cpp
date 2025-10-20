#include "rppch.h"
#include "Renderer.hpp"

#include "Rapid/Core/Core.hpp"
#include "Rapid/Core/Logger.hpp"

#include "Rapid/Core/Window.hpp"

namespace Rapid::Project
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Constructor & Destructor
    ////////////////////////////////////////////////////////////////////////////////////
    Renderer::Renderer(Window& target, uint32_t projectWidth, uint32_t projectHeight)
        : m_TargetWindow(target), m_Width(projectWidth), m_Height(projectHeight)
    {
        // Create target images
        {
            Obsidian::ImageSpecification imageSpec = GetInternalRenderer().GetSwapChain().GetImage(0).GetSpecification();
            imageSpec.SetWidthAndHeight(projectWidth, projectHeight);
            imageSpec.SetIsShaderResource(true);
            imageSpec.SetIsRenderTarget(true);
            imageSpec.SetPermanentState(Obsidian::ResourceState::ShaderResource);

            for (auto& image : m_Images)
            {
                image.Construct(GetInternalRenderer().GetDevice(), imageSpec);
                GetInternalRenderer().GetDevice().StartTracking(image.Get());
            }
        }

        m_2DRenderer.Construct(*this);
        m_3DRenderer.Construct(*this);
        m_UIRenderer.Construct(*this);
    }

    Renderer::~Renderer()
    {
        for (auto& image : m_Images)
            GetInternalRenderer().GetDevice().DestroyImage(image.Get());
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Methods
    ////////////////////////////////////////////////////////////////////////////////////
    void Renderer::Render(const Scene2D& scene)
    {
        m_2DRenderer->Render(scene);
        m_UIRenderer->Render(m_2DRenderer->GetCommandList(GetCurrentFrame()));
    }

    void Renderer::Render(const Scene3D& scene)
    {
        m_3DRenderer->Render(scene);
        m_UIRenderer->Render(m_3DRenderer->GetCommandList(GetCurrentFrame()));
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Methods
    ////////////////////////////////////////////////////////////////////////////////////
    void Renderer::Resize(uint32_t width, uint32_t height)
    {
        for (auto& image : m_Images)
            image->Resize(width, height);

        m_2DRenderer->Resize();
        m_3DRenderer->Resize();
        m_UIRenderer->Resize();
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Getters
    ////////////////////////////////////////////////////////////////////////////////////
    uint8_t Renderer::GetCurrentFrame() const
    {
        return GetInternalRenderer().GetCurrentFrame();
    }

    Rapid::Renderer& Renderer::GetInternalRenderer()
    {
        return m_TargetWindow.m_Renderer;
    }

    const Rapid::Renderer& Renderer::GetInternalRenderer() const
    {
        return m_TargetWindow.m_Renderer;
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Private methods
    ////////////////////////////////////////////////////////////////////////////////////

}