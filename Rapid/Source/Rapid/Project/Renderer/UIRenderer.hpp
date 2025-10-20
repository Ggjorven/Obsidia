#pragma once

#include "Rapid/Core/Core.hpp"
#include "Rapid/Core/Logger.hpp"

#include <Obsidian/Core/Information.hpp>
#include <Obsidian/Renderer/Renderpass.hpp>
#include <Obsidian/Renderer/CommandList.hpp>

#include <cstdint>

namespace Rapid::Project
{

    class Renderer;

    ////////////////////////////////////////////////////////////////////////////////////
    // UIRenderer // Note: This UIRenderer refers to the UI made by the game developer, not the engine UI
    ////////////////////////////////////////////////////////////////////////////////////
    class UIRenderer
    {
    public:
        // Constructor & Destructor
        UIRenderer(Renderer& targetRenderer);
        ~UIRenderer();

        // Methods
        void Render(const Obsidian::CommandList& waitOn);

        void Resize();

        // Getters
        inline const Obsidian::CommandList& GetCommandList(uint8_t frame) const { RP_ASSERT((frame < m_CommandLists.size()), "Frame exceeds size."); return m_CommandLists[frame].Get(); }

    private:
        // Private methods
        void Begin();
        void RenderUI();
        void End(const Obsidian::CommandList& waitOn);

    private:
        Renderer& m_TargetRenderer;

        std::array<Nano::Memory::DeferredConstruct<Obsidian::CommandList>, Obsidian::Information::FramesInFlight> m_CommandLists = { };
        Obsidian::Renderpass m_Renderpass;
    };

}