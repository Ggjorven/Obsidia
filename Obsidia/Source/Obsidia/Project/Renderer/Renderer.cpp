#include "obpch.h"
#include "Renderer.hpp"

#include "Obsidia/Core/Core.hpp"
#include "Obsidia/Core/Logger.hpp"

#include "Obsidia/Core/Window.hpp"

namespace Ob::Project
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Constructor & Destructor
    ////////////////////////////////////////////////////////////////////////////////////
    Renderer::Renderer(Window& target, uint32_t projectWidth, uint32_t projectHeight)
        : m_TargetWindow(target), m_Width(projectWidth), m_Height(projectHeight)
    {
        for (auto& image : m_Images)
            image.Construct(GetInternalRenderer().GetDevice(), GetInternalRenderer().GetSwapChain().GetImage(0).GetSpecification());
    }

    Renderer::~Renderer()
    {
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Methods
    ////////////////////////////////////////////////////////////////////////////////////
    void Renderer::Begin()
    {
        m_LastVisualLayer = nullptr;
    }

    void Renderer::End()
    {
    }

    void Renderer::BeginLayer(VisualLayer& layer)
    {
        layer.Begin();
    }

    void Renderer::EndLayer(VisualLayer& layer)
    {
        if (m_LastVisualLayer)
        {
            std::vector<const Obsidian::CommandList*> waitOn{ m_LastVisualLayer };
            layer.End(waitOn);
        }
        else
            layer.End({});

        m_LastVisualLayer = &layer.GetCommandList(GetCurrentFrame());
    }

    void Renderer::Resize(uint32_t width, uint32_t height)
    {
        for (auto& image : m_Images)
            image->Resize(width, height);
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Getters
    ////////////////////////////////////////////////////////////////////////////////////
    uint8_t Renderer::GetCurrentFrame() const
    {
        return GetInternalRenderer().GetCurrentFrame();
    }

    Ob::Renderer& Renderer::GetInternalRenderer()
    {
        return m_TargetWindow.m_Renderer;
    }

    const Ob::Renderer& Renderer::GetInternalRenderer() const
    {
        return m_TargetWindow.m_Renderer;
    }

}