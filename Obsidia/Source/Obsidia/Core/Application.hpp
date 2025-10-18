#pragma once

#include "Obsidia/Core/Core.hpp"

#include "Obsidia/Project/Project.hpp"
#include "Obsidia/Renderer/Renderer.hpp"

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
        // Window
        uint32_t Width = 0, Height = 0;
        std::string Title = {};
        // FUTURE TODO: Window flags

        // Project/Engine
        Project::ProjectSpecification Project = {};

    public:
        // Setters
        inline constexpr ApplicationSpecification& SetWidth(uint32_t width) { Width = width; return *this; }
        inline constexpr ApplicationSpecification& SetHeight(uint32_t height) { Height = height; return *this; }
        inline constexpr ApplicationSpecification& SetWidthAndHeight(uint32_t width, uint32_t height) { Width = width; Height = height; return *this; }
        inline ApplicationSpecification& SetTitle(const std::string& title) { Title = title; return *this; }

        inline ApplicationSpecification& SetProject(const Project::ProjectSpecification& specs) { Project = specs; return *this; }
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
        inline Obsidian::Window& GetWindow() { return m_Window; }

    private:
        // Private methods
        void OnEvent(Obsidian::Event e);

    private:
        ApplicationSpecification m_Specification;

        // Engine/Window
        Obsidian::Window m_Window;
        Nano::Memory::DeferredConstruct<Renderer, true> m_Renderer;

        // Project
        std::shared_ptr<Project::Project> m_Project;
    };

}