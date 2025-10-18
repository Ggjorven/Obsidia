#pragma once

#include "Obsidia/Core/Core.hpp"
#include "Obsidia/Core/Logger.hpp"

#include "Obsidia/Project/Renderer/VisualLayer.hpp"
#include "Obsidia/Project/Renderer/Scene2DRenderer.hpp"
#include "Obsidia/Project/Renderer/Scene3DRenderer.hpp"

#include <Obsidian/Core/Information.hpp>
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

        // Static methods
        // Note: These functions are called by the window
        static void Begin();
        static void End();

        // Note: These functions are called by the project/scene
        static void BeginLayer(Project::VisualLayer& layer);
        static void EndLayer(Project::VisualLayer& layer);

        // Other
        static void Resize(uint32_t width, uint32_t height);

        // Static getters
        inline static Obsidian::Window& GetWindow() { return s_Instance->m_Window; } // Note: This is here, because we can't have it in Application because the Project/Scene already requires the window in that initialization
        inline static Obsidian::Device& GetDevice() { return s_Instance->m_Device; }
        inline static Obsidian::Swapchain& GetSwapChain() { return s_Instance->m_SwapChain; }
        inline static Obsidian::CommandListPool& GetGraphicsPool(uint8_t frame) { OBSIDIA_ASSERT((frame < s_Instance->m_GraphicsListPools.size()), "Frame exceeds pool size."); return s_Instance->m_GraphicsListPools[frame]; }

        inline static uint8_t GetCurrentFrame() { return s_Instance->m_SwapChain.GetCurrentFrame(); }

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
        std::array<Nano::Memory::DeferredConstruct<Obsidian::CommandList>, Obsidian::Information::FramesInFlight> m_CompositeLists = { };
        Obsidian::Renderpass m_CompositePass;

        Obsidian::CommandList const* m_LastVisualLayerList = nullptr;

        // TODO: 2D & 3D renderer

        std::queue<Obsidian::DeviceDestroyFn> m_DestroyQueue = {};

        inline static Renderer* s_Instance = nullptr;
    };

}