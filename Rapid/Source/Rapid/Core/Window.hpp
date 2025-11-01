#pragma once

#include "Rapid/Core/Core.hpp"

#include "Rapid/Project/Project.hpp"
#include "Rapid/Renderer/Renderer.hpp"

#include <Obsidian/Core/Window.hpp>

#include <cstdint>
#include <string>
#include <memory>
#include <functional>

namespace Rapid::Project
{
    class Renderer;
}

namespace Rapid
{

    ////////////////////////////////////////////////////////////////////////////////////
    // WindowSpecification
    ////////////////////////////////////////////////////////////////////////////////////
    struct WindowSpecification
    {
    public:
        using EventCallbackFn = std::function<void(const Obsidian::Event& e)>;
    public:
        uint32_t Width = 0, Height = 0;
        std::string Title = {};
        bool VSync = false;

        Obsidian::WindowFlags Flags = Obsidian::WindowFlags::Default;

        EventCallbackFn EventCallback = nullptr;

    public:
        // Setters
        inline constexpr WindowSpecification& SetWidth(uint32_t width) { Width = width; return *this; }
        inline constexpr WindowSpecification& SetHeight(uint32_t height) { Height = height; return *this; }
        inline constexpr WindowSpecification& SetWidthAndHeight(uint32_t width, uint32_t height) { Width = width; Height = height; return *this; }
        inline WindowSpecification& SetTitle(const std::string& title) { Title = title; return *this; }

        inline constexpr WindowSpecification& AddFlags(Obsidian::WindowFlags flags) { Flags |= flags; return *this; }
        inline constexpr WindowSpecification& SetFlags(Obsidian::WindowFlags flags) { Flags = flags; return *this; }

        inline WindowSpecification& SetEventCallback(const EventCallbackFn& fn) { EventCallback = fn; return *this; }
    };

    ////////////////////////////////////////////////////////////////////////////////////
    // Window
    ////////////////////////////////////////////////////////////////////////////////////
    class Window
    {
    public:
        // Constructor & Destructor
        Window(const WindowSpecification& specs);
        ~Window();

        // Methods
        void SwapBuffers();
        void PollEvents();

        // Getters
        inline const WindowSpecification& GetSpecification() const { return m_Specification; }
        inline Obsidian::Window& GetWindow() { return m_Window; }

        inline bool IsOpen() const { return m_Window.IsOpen(); }

        inline uint32_t GetWidth() const { return m_Window.GetSize().x; }
        inline uint32_t GetHeight() const { return m_Window.GetSize().y; }

        inline float GetTime() const { return static_cast<float>(m_Window.GetWindowTime()); }

    private:
        // Private methods
        void OnEvent(Obsidian::Event e);

    private:
        WindowSpecification m_Specification;

        Obsidian::Window m_Window;
        Renderer m_Renderer;

        friend class Renderer;
        friend class Project::Renderer;
    };

}