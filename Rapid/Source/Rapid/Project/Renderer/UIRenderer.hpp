#pragma once

#include "Rapid/Core/Core.hpp"
#include "Rapid/Core/Logger.hpp"

#include <Obsidian/Core/Information.hpp>
#include <Obsidian/Renderer/Renderpass.hpp>
#include <Obsidian/Renderer/CommandList.hpp>

#include <cstdint>

namespace Rapid
{
    class Renderer;
}

namespace Rapid::Project
{

    ////////////////////////////////////////////////////////////////////////////////////
    // UIRenderer // Note: This UIRenderer refers to the UI made by the game developer, not the engine UI
    ////////////////////////////////////////////////////////////////////////////////////
    class UIRenderer
    {
    public:
        // Constructor & Destructor
        UIRenderer(Rapid::Renderer& internalRenderer, std::span<Obsidian::Image*, Obsidian::Information::FramesInFlight> images);
        ~UIRenderer();

        // Methods
        void Render(const Obsidian::CommandList& waitOn, bool waitForSwapchain, bool onFinishMakeSwapchainPresentable);

        void Resize(uint32_t width, uint32_t height);

        // Getters
        inline const Obsidian::CommandList& GetCommandList(uint8_t frame) const { RP_ASSERT((frame < m_CommandLists.size()), "Frame exceeds size."); return m_CommandLists[frame].Get(); }

    private:
        // Private methods
        void Begin();
        void RenderUI();
        void End(const Obsidian::CommandList& waitOn, bool waitForSwapchain, bool onFinishMakeSwapchainPresentable);

    private:
        Rapid::Renderer& m_InternalRenderer;
        uint32_t m_Width, m_Height;

        std::array<Nano::Memory::DeferredConstruct<Obsidian::CommandList>, Obsidian::Information::FramesInFlight> m_CommandLists = { };
        Obsidian::Renderpass m_Renderpass;
    };

}