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
#include <optional>

namespace Rapid
{
    class Window;
}

namespace Rapid::Project
{

    class Project;
    class Scene2D;
    class Scene3D;

    ////////////////////////////////////////////////////////////////////////////////////
    // Renderer
    ////////////////////////////////////////////////////////////////////////////////////
    class Renderer
    {
    public:
        // Constructor & Destructor
        Renderer(Window& target); 
		Renderer(Window& target, uint32_t projectWidth, uint32_t projectHeight); // Note: Renders to custom image // TODO: FIX
        ~Renderer();

        // Methods // Note: Returns the acquired frame
        uint8_t Render(const Project& project);
        uint8_t Render(const Scene2D& scene);
        uint8_t Render(const Scene3D& scene);

        void Resize(uint32_t width, uint32_t height);

        // Getters
        uint32_t GetWidth() const;
        uint32_t GetHeight() const;
        
        Obsidian::Image& GetImage(uint8_t frame);
        uint8_t GetCurrentFrame() const;

        inline Window& GetTargetWindow() { return m_TargetWindow; }
        Rapid::Renderer& GetInternalRenderer();
        const Rapid::Renderer& GetInternalRenderer() const;

    private:
        Window& m_TargetWindow;

        // Note: This is only set when the constructor with width and height are used.
        std::optional<std::array<Nano::Memory::DeferredConstruct<Obsidian::Image, true>, Obsidian::Information::FramesInFlight>> m_Images = { };

        Nano::Memory::DeferredConstruct<Scene2DRenderer> m_2DRenderer = {};
        Nano::Memory::DeferredConstruct<Scene3DRenderer> m_3DRenderer = {};
        Nano::Memory::DeferredConstruct<UIRenderer> m_UIRenderer = {};
    };

}
