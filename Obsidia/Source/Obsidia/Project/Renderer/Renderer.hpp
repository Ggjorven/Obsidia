#pragma once

#include "Obsidia/Core/Core.hpp"
#include "Obsidia/Core/Logger.hpp"

#include "Obsidia/Project/Renderer/VisualLayer.hpp"
#include "Obsidia/Project/Renderer/Scene2DRenderer.hpp"
#include "Obsidia/Project/Renderer/Scene3DRenderer.hpp"

#include "Obsidia/Renderer/Renderer.hpp"

#include <Obsidian/Core/Information.hpp>
#include <Obsidian/Renderer/Image.hpp>
#include <Obsidian/Renderer/CommandList.hpp>

#include <cstdint>
#include <array>

namespace Ob
{
    class Window;
}

namespace Ob::Project
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Renderer
    ////////////////////////////////////////////////////////////////////////////////////
    class Renderer
    {
    public:
        // Constructor & Destructor
        Renderer(Window& target, uint32_t projectWidth, uint32_t projectHeight);
        ~Renderer();

        // Methods
        void Begin();
        void End();

        void BeginLayer(VisualLayer& layer);
        void EndLayer(VisualLayer& layer);

        void Resize(uint32_t width, uint32_t height);

        // Getters
        inline uint32_t GetWidth() const { return m_Width; }
        inline uint32_t GetHeight() const { return m_Height; }
        inline Obsidian::Image& GetImage(uint8_t frame) { OBSIDIA_ASSERT((frame < m_Images.size()), "Frame exceeds image count."); return m_Images[frame].Get(); }
        
        uint8_t GetCurrentFrame() const;

        inline Window& GetTargetWindow() { return m_TargetWindow; }
        Ob::Renderer& GetInternalRenderer();
        const Ob::Renderer& GetInternalRenderer() const;

    private:
        // Private methods

    private:
        Window& m_TargetWindow;
        uint32_t m_Width, m_Height;
        std::array<Nano::Memory::DeferredConstruct<Obsidian::Image>, Obsidian::Information::FramesInFlight> m_Images = { };

        Obsidian::CommandList const* m_LastVisualLayer = nullptr;

        Scene2DRenderer m_2DRenderer;
        Scene3DRenderer m_3DRenderer;
    };

}