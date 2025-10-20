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
    class Scene2D;

    ////////////////////////////////////////////////////////////////////////////////////
    // Scene2DRenderer
    ////////////////////////////////////////////////////////////////////////////////////
    class Scene2DRenderer
    {
    public:
        // Constructor & Destructor
        Scene2DRenderer(Renderer& targetRenderer);
        ~Scene2DRenderer();

        // Methods
        void Render(const Scene2D& scene);

        void Resize(); // Note: Resizes to Image's width and height, so no need to pass in.

        // Getters
        inline const Obsidian::CommandList& GetCommandList(uint8_t frame) const { RP_ASSERT((frame < m_CommandLists.size()), "Frame exceeds size."); return m_CommandLists[frame].Get(); }

    private:
        // Private methods
        void Begin();
        void RenderScene(const Scene2D& scene);
        void End();

    private:
        Renderer& m_TargetRenderer;

        std::array<Nano::Memory::DeferredConstruct<Obsidian::CommandList>, Obsidian::Information::FramesInFlight> m_CommandLists = { };
        Obsidian::Renderpass m_Renderpass;
    };

}