#pragma once

#include "Rapid/Core/Core.hpp"
#include "Rapid/Core/Logger.hpp"

#include "Rapid/Renderer/Renderer.hpp"

#include "Rapid/Project/Renderer/Scene2DRenderer.hpp"
#include "Rapid/Project/Renderer/Scene3DRenderer.hpp"
#include "Rapid/Project/Renderer/UIRenderer.hpp"

#include <Obsidian/Core/Information.hpp>
#include <Obsidian/Renderer/Image.hpp>
#include <Obsidian/Renderer/CommandList.hpp>

#include <cstdint>
#include <array>

namespace Rapid
{
    class Window;
}

namespace Rapid::Project
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
        void Render(const Scene2D& scene);
        void Render(const Scene3D& scene);

        void Resize(uint32_t width, uint32_t height);

        // Getters
        inline uint32_t GetWidth() const { return m_Width; }
        inline uint32_t GetHeight() const { return m_Height; }
        inline Obsidian::Image& GetImage(uint8_t frame) { RP_ASSERT((frame < m_Images.size()), "Frame exceeds image count."); return m_Images[frame].Get(); }
        
        uint8_t GetCurrentFrame() const;

        inline Window& GetTargetWindow() { return m_TargetWindow; }
        Rapid::Renderer& GetInternalRenderer();
        const Rapid::Renderer& GetInternalRenderer() const;

    private:
        Window& m_TargetWindow;
        uint32_t m_Width, m_Height;
        std::array<Nano::Memory::DeferredConstruct<Obsidian::Image>, Obsidian::Information::FramesInFlight> m_Images = { };

        Nano::Memory::DeferredConstruct<Scene2DRenderer> m_2DRenderer = {};
        Nano::Memory::DeferredConstruct<Scene3DRenderer> m_3DRenderer = {};
        Nano::Memory::DeferredConstruct<UIRenderer> m_UIRenderer = {};
    };

}