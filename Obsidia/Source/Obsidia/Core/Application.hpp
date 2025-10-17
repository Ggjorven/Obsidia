#pragma once

#include "Obsidia/Core/Core.hpp"

#include "Obsidia/Project/Project.hpp"

#include <Obsidian/Core/Window.hpp>

#include <cstdint>
#include <string>
#include <memory>
#include <functional>

namespace Ob
{

    ////////////////////////////////////////////////////////////////////////////////////
    // ApplicationSpecification
    ////////////////////////////////////////////////////////////////////////////////////
    struct ApplicationSpecification
    {
    public:
        using UpdateFn = std::function<void(float deltaTime)>;
        using RenderFn = std::function<void()>;
        using EventFn = std::function<void(const Obsidian::Event& e)>;
    public:
        // Window
        uint32_t Width = 0, Height = 0;
        std::string Title = {};
        // FUTURE TODO: Window flags

        UpdateFn UpdateCallback = nullptr;
        RenderFn RenderCallback = nullptr;
        EventFn EventCallback = nullptr;

        // Project/Engine
        std::shared_ptr<Project::Project> Project = nullptr;

    public:
        // Setters
        inline constexpr ApplicationSpecification& SetWidth(uint32_t width) { Width = width; return *this; }
        inline constexpr ApplicationSpecification& SetHeight(uint32_t height) { Height = height; return *this; }
        inline constexpr ApplicationSpecification& SetWidthAndHeight(uint32_t width, uint32_t height) { Width = width; Height = height; return *this; }
        inline ApplicationSpecification& SetTitle(const std::string& title) { Title = title; return *this; }

        inline ApplicationSpecification& SetUpdateCallback(const UpdateFn& updateCb) { UpdateCallback = updateCb; return *this; }
        inline ApplicationSpecification& SetRenderCallback(const RenderFn& renderCb) { RenderCallback = renderCb; return *this; }
        inline ApplicationSpecification& SetEventCallback(const EventFn& eventCb) { EventCallback = eventCb; return *this; }

        inline ApplicationSpecification& SetProject(const Project::ProjectSpecification& specs) { Project = std::make_shared<Project::Project>(specs); return *this; }
    };

    ////////////////////////////////////////////////////////////////////////////////////
    // Application
    ////////////////////////////////////////////////////////////////////////////////////
    class Application
    {
    public:
        // Constructor & Destructor
        Application(const ApplicationSpecification& specs);
        ~Application();

        // Methods
        void Run();

        // Getters
        inline const ApplicationSpecification& GetSpecification() { return m_Specification; }

    private:
        // Private methods
        void OnEvent(Obsidian::Event e);

    private:
        ApplicationSpecification m_Specification;
        Obsidian::Window m_Window;
    };

}