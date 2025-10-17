#pragma once

#include "Obsidia/Core/Core.hpp"

#include "Obsidia/Project/Renderer/VisualLayer.hpp"
#include "Obsidia/Project/Renderer/Scene2DRenderer.hpp"
#include "Obsidia/Project/Renderer/Scene3DRenderer.hpp"

#include <Obsidian/Core/Window.hpp>

#include <Obsidian/Renderer/Device.hpp>
#include <Obsidian/Renderer/SwapChain.hpp>

#include <cstdint>
#include <vector>

namespace Ob
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

        // Note: These functions are called by the project/scene
        void BeginLayer(const Project::VisualLayer& layer);
        void EndLayer(const Project::VisualLayer& layer);

        // Other
        void Resize(uint32_t width, uint32_t height);

    private:
        // Private methods
        void DestroyQueue();

        // Callbacks
        void DestroyCallback(Obsidian::DeviceDestroyFn fn);
        void MessageCallback(Obsidian::DeviceMessageType type, const std::string& message) const;

        // Static getters
        // Note: To not make both renderers when not needed we use a static getter that
        // only initializes when it is actually called (so we're actually using it)
        static Project::Scene2DRenderer& Get2DRenderer();
        static Project::Scene3DRenderer& Get3DRenderer();

    private:
        Obsidian::Window& m_Window;
        Obsidian::Device m_Device;
        Obsidian::Swapchain m_SwapChain;

        std::queue<Obsidian::DeviceDestroyFn> m_DestroyQueue = {};
    };

}