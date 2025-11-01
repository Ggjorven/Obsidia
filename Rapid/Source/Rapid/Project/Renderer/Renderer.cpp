#include "rppch.h"
#include "Renderer.hpp"

#include "Rapid/Core/Core.hpp"
#include "Rapid/Core/Logger.hpp"

#include "Rapid/Core/Window.hpp"

#include "Rapid/Project/Project.hpp"

namespace Rapid::Project
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Constructor & Destructor
    ////////////////////////////////////////////////////////////////////////////////////
   	Renderer::Renderer(Window& target)
        : m_TargetWindow(target)
    {
        // Create a span of swapchain images
        std::array<Obsidian::Image*, Obsidian::Information::FramesInFlight> images = { };
        for (uint8_t i = 0; i < Obsidian::Information::FramesInFlight; i++)
            images[i] = &GetInternalRenderer().GetSwapchain().GetImage(i);

        m_2DRenderer.Construct(GetInternalRenderer(), std::span<Obsidian::Image*, Obsidian::Information::FramesInFlight>(images));
        m_3DRenderer.Construct(GetInternalRenderer(), std::span<Obsidian::Image*, Obsidian::Information::FramesInFlight>(images));
        m_UIRenderer.Construct(GetInternalRenderer(), std::span<Obsidian::Image*, Obsidian::Information::FramesInFlight>(images));
    } 

	Renderer::Renderer(Window& target, uint32_t projectWidth, uint32_t projectHeight)
        : m_TargetWindow(target)
    {
        // Create target images
        {
            Obsidian::ImageSpecification imageSpec = GetInternalRenderer().GetSwapchain().GetImage(0).GetSpecification();
            imageSpec.SetWidthAndHeight(projectWidth, projectHeight);
            imageSpec.SetIsShaderResource(true);
            imageSpec.SetIsRenderTarget(true);

            for (auto& image : m_Images.emplace()) // Note: Creates custom images
            {
                image.Construct(GetInternalRenderer().GetDevice(), imageSpec);
                GetInternalRenderer().GetDevice().StartTracking(image.Get(), Obsidian::ImageSubresourceSpecification(), Obsidian::ResourceState::Unknown);
            }
        }

        // Create a span of images
        std::array<Obsidian::Image*, Obsidian::Information::FramesInFlight> images = { };
        for (uint8_t i = 0; i < Obsidian::Information::FramesInFlight; i++)
            images[i] = &m_Images.value()[i].Get();

        m_2DRenderer.Construct(GetInternalRenderer(), std::span<Obsidian::Image*, Obsidian::Information::FramesInFlight>(images));
        m_3DRenderer.Construct(GetInternalRenderer(), std::span<Obsidian::Image*, Obsidian::Information::FramesInFlight>(images));
        m_UIRenderer.Construct(GetInternalRenderer(), std::span<Obsidian::Image*, Obsidian::Information::FramesInFlight>(images));
    }

    Renderer::~Renderer()
    {
        if (m_Images.has_value()) // If we have a custom image we're rendering to destroy it
        {
            for (auto& image : m_Images.value())
                GetInternalRenderer().GetDevice().DestroyImage(image.Get());
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Methods
    ////////////////////////////////////////////////////////////////////////////////////
    uint8_t Renderer::Render(const Project& project)
    {
        if (project.GetActiveScene()->Is2D())
            return Render(project.GetActiveScene()->Get2D());
        else if (project.GetActiveScene()->Is3D())
            return Render(project.GetActiveScene()->Get3D());

        RP_ASSERT(false, "Unreachable.");
        return 0;
    }

    uint8_t Renderer::Render(const Scene2D& scene)
    {
        GetInternalRenderer().Begin();
        m_2DRenderer->Render(scene, !m_Images.has_value(), false);
        m_UIRenderer->Render(m_2DRenderer->GetCommandList(GetCurrentFrame()), false, !m_Images.has_value());
        GetInternalRenderer().End();

        return GetInternalRenderer().GetSwapchain().GetAcquiredImage();
    }

    uint8_t Renderer::Render(const Scene3D& scene)
    {
        GetInternalRenderer().Begin();
        m_3DRenderer->Render(scene, !m_Images.has_value(), false);
        m_UIRenderer->Render(m_3DRenderer->GetCommandList(GetCurrentFrame()), false, !m_Images.has_value());
        GetInternalRenderer().End();

        return GetInternalRenderer().GetSwapchain().GetAcquiredImage();
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Methods
    ////////////////////////////////////////////////////////////////////////////////////
    void Renderer::Resize(uint32_t width, uint32_t height)
    {
        // If we're rendering to a custom image, resize it
        if (m_Images.has_value())
        {
            for (auto& image : m_Images.value())
                image->Resize(width, height);
        }
        else // Else resize the swapchain images
        {
            GetInternalRenderer().Resize(width, height);
        }

        m_2DRenderer->Resize(width, height);
        m_3DRenderer->Resize(width, height);
        m_UIRenderer->Resize(width, height);
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Getters
    ////////////////////////////////////////////////////////////////////////////////////
    uint32_t Renderer::GetWidth() const
    {
        if (m_Images.has_value())
        {
            return m_Images.value()[0]->GetSpecification().Width;
        }
        else
        {
            return m_TargetWindow.m_Renderer.GetSwapchain().GetImage(0).GetSpecification().Width;
        }
    }

    uint32_t Renderer::GetHeight() const
    {
        if (m_Images.has_value())
        {
            return m_Images.value()[0]->GetSpecification().Height;
        }
        else
        {
            return m_TargetWindow.m_Renderer.GetSwapchain().GetImage(0).GetSpecification().Height;
        }
    }

    Obsidian::Image& Renderer::GetImage(uint8_t frame) 
    { 
        if (m_Images.has_value())
        {
            RP_ASSERT((frame < m_Images.value().size()), "Frame exceeds image count.");
            return m_Images.value()[frame].Get();
        }
        else
        {
            // Note: Return swapchain image
            RP_ASSERT((frame < Obsidian::Information::FramesInFlight), "Frame exceeds image count.");
            return m_TargetWindow.m_Renderer.GetSwapchain().GetImage(frame);
        }
    }

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
