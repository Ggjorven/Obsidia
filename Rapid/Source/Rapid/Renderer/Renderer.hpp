#pragma once

#include "Rapid/Core/Core.hpp"
#include "Rapid/Core/Logger.hpp"

#include "Rapid/Project/Renderer/VisualLayer.hpp"
#include "Rapid/Project/Renderer/Scene2DRenderer.hpp"
#include "Rapid/Project/Renderer/Scene3DRenderer.hpp"

#include <Obsidian/Core/Information.hpp>
#include <Obsidian/Core/Window.hpp>

#include <Obsidian/Renderer/Device.hpp>
#include <Obsidian/Renderer/SwapChain.hpp>

#include <cstdint>
#include <vector>

namespace Rapid::Project
{
    class Renderer;
}

namespace Rapid
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Renderer
    ////////////////////////////////////////////////////////////////////////////////////
    class Renderer
    {
    public:
        // Constructor & Destructor
        Renderer(Obsidian::Window& window, bool vsync);
        ~Renderer();

        // Methods
        // Note: These functions are called by the window
        void Begin();
        void End();

        // Other
        void Resize(uint32_t width, uint32_t height);

        // Getters
        inline Obsidian::Window& GetWindow() { return m_Window; } // Note: This is here, because we can't have it in Application because the Project/Scene already requires the window in that initialization
        inline Obsidian::Device& GetDevice() { return m_Device; }
        inline Obsidian::Swapchain& GetSwapChain() { return m_SwapChain; }
        inline Obsidian::CommandListPool& GetGraphicsPool(uint8_t frame) { RP_ASSERT((frame < m_GraphicsListPools.size()), "Frame exceeds pool size."); return m_GraphicsListPools[frame]; }

        inline uint8_t GetCurrentFrame() const { return m_SwapChain.GetCurrentFrame(); }

    private:
        // Private methods
        void DestroyQueue();

        // Callbacks
        void DestroyCallback(Obsidian::DeviceDestroyFn fn);
        void MessageCallback(Obsidian::DeviceMessageType type, const std::string& message) const;

    private:
        Obsidian::Window& m_Window;
        Obsidian::Device m_Device;
        Obsidian::Swapchain m_SwapChain;

        // Combines all visual layers
        std::array<Nano::Memory::DeferredConstruct<Obsidian::CommandListPool>, Obsidian::Information::FramesInFlight> m_GraphicsListPools = { };

        std::queue<Obsidian::DeviceDestroyFn> m_DestroyQueue = {};

        friend class Project::Renderer;
    };

}