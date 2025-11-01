#pragma once

#include "Rapid/Core/Core.hpp"
#include "Rapid/Core/Logger.hpp"

#include <Obsidian/Core/Information.hpp>
#include <Obsidian/Renderer/Renderpass.hpp>
#include <Obsidian/Renderer/CommandList.hpp>

#include <cstdint>
#include <span>

namespace Rapid
{
    class Renderer;
}

namespace Rapid::Project
{

    class Scene2D;

    ////////////////////////////////////////////////////////////////////////////////////
    // Scene2DRenderer
    ////////////////////////////////////////////////////////////////////////////////////
    class Scene2DRenderer
    {
    public:
        // Constructor & Destructor
        Scene2DRenderer(Rapid::Renderer& internalRenderer, std::span<Obsidian::Image*, Obsidian::Information::FramesInFlight> images);
        ~Scene2DRenderer();

        // Methods
        void Render(const Scene2D& scene, bool waitForSwapchain, bool onFinishMakeSwapchainPresentable);

        void Resize(uint32_t width, uint32_t height);

        // Getters
        inline const Obsidian::CommandList& GetCommandList(uint8_t frame) const { RP_ASSERT((frame < m_CommandLists.size()), "Frame exceeds size."); return m_CommandLists[frame].Get(); }

    private:
        // Private methods
        void Begin();
        void RenderScene(const Scene2D& scene);
        void End(bool waitForSwapchain, bool onFinishMakeSwapchainPresentable);

    private:
        Rapid::Renderer& m_InternalRenderer;
        uint32_t m_Width, m_Height;

        std::array<Nano::Memory::DeferredConstruct<Obsidian::CommandList>, Obsidian::Information::FramesInFlight> m_CommandLists = { };
        Obsidian::Renderpass m_Renderpass;
    };

}