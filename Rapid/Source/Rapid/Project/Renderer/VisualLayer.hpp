#pragma once

#include "Rapid/Core/Core.hpp"
#include "Rapid/Core/Logger.hpp"

#include <Obsidian/Renderer/CommandList.hpp>
#include <Obsidian/Renderer/Renderpass.hpp>

#include <cstdint>
#include <span>

namespace Rapid::Project
{

    class Scene;
    class Renderer;

    ////////////////////////////////////////////////////////////////////////////////////
    // VisualLayerSpecification
    ////////////////////////////////////////////////////////////////////////////////////
    struct VisualLayerSpecification
    {
    public:
        uint8_t Level = 0; // Note: 0, means auto initialize

    public:
        // Setters
        inline constexpr VisualLayerSpecification& SetLevel(uint8_t level) { Level = level; return *this; }
    };

    ////////////////////////////////////////////////////////////////////////////////////
    // VisualLayer
    ////////////////////////////////////////////////////////////////////////////////////
    class VisualLayer
    {
    public:
        // Constructor & Destructor
        VisualLayer(Renderer& targetRenderer, const VisualLayerSpecification& specs);
        ~VisualLayer();

        // Methods
        void Begin();
        void End(std::span<const Obsidian::CommandList*> waitOn);

        // Note: Resizes to underlying image size
        void Resize();

        // Getters
        inline const VisualLayerSpecification& GetSpecification() { return m_Specification; }
        inline const Obsidian::CommandList& GetCommandList(uint8_t frame) const { RP_ASSERT((frame < m_CommandLists.size()), "Frame exceeds size."); return m_CommandLists[frame].Get(); }

    private:
        // Private methods

    private:
        Renderer& m_TargetRenderer;
        VisualLayerSpecification m_Specification;

        std::array<Nano::Memory::DeferredConstruct<Obsidian::CommandList>, Obsidian::Information::FramesInFlight> m_CommandLists = { };
        Obsidian::Renderpass m_Renderpass;

        friend class Scene;
    };

}