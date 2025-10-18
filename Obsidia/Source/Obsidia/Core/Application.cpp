#include "obpch.h"
#include "Application.hpp"

#include "Obsidia/Core/Core.hpp"
#include "Obsidia/Core/Logger.hpp"

#include "Obsidia/Project/Events.hpp"

namespace Ob
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Constructor & Destructor
    ////////////////////////////////////////////////////////////////////////////////////
    Application::Application(const ApplicationSpecification& specs)
        : m_Specification(specs)
        , m_Window(Obsidian::WindowSpecification()
            .SetTitle(specs.Title)
            .SetWidthAndHeight(specs.Width, specs.Height)
            .SetFlags(Obsidian::WindowFlags::Default)
            .SetEventCallback([this](Obsidian::Event e) { OnEvent(e); })
        )
        , m_Renderer(m_Window, true) // TODO: Make VSync a parameter
        , m_Project(new Project::Project(specs.Project))
    {
    }

    Application::~Application()
    {
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Methods
    ////////////////////////////////////////////////////////////////////////////////////
    void Application::Run()
    {
        float lastTime = static_cast<float>(m_Window.GetWindowTime());

        while (m_Window.IsOpen())
        {
            // Update
            {
                float currentTime = static_cast<float>(m_Window.GetWindowTime());
                float deltaTime = currentTime - lastTime;
                m_Project->OnUpdate(deltaTime);
                lastTime = static_cast<float>(currentTime);
            }

            // Render
            {
                m_Renderer->Begin();
                m_Project->OnRender();
                m_Renderer->End();
            }

            m_Window.SwapBuffers();
            m_Window.PollEvents();
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Private methods
    ////////////////////////////////////////////////////////////////////////////////////
    void Application::OnEvent(Obsidian::Event e)
    {
        Nano::Events::EventHandler handler(e);

        // Window/Application
        {
            handler.Handle<Obsidian::WindowCloseEvent>([&](const Obsidian::WindowCloseEvent& wce) { (void)wce; m_Window.Close();  });
            handler.Handle<Obsidian::WindowResizeEvent>([&](const Obsidian::WindowResizeEvent& wre) { m_Renderer->Resize(wre.GetWidth(), wre.GetHeight()); });
        }

        // Engine/Project
        // FUTURE TODO: Have a custom area for the project (area as in custom size)
        {
            handler.Handle<Obsidian::WindowResizeEvent>([&](const Obsidian::WindowResizeEvent& wre) { m_Project->OnEvent(Project::ResizeEvent(wre)); });

            handler.Handle<Obsidian::KeyPressedEvent>([&](const Obsidian::KeyPressedEvent& kpe) { m_Project->OnEvent(Project::KeyPressedEvent(kpe)); });
            handler.Handle<Obsidian::KeyReleasedEvent>([&](const Obsidian::KeyReleasedEvent& kre) { m_Project->OnEvent(Project::KeyReleasedEvent(kre)); });

            handler.Handle<Obsidian::MouseMovedEvent>([&](const Obsidian::MouseMovedEvent& mme) { m_Project->OnEvent(Project::MouseMovedEvent(mme)); });
            handler.Handle<Obsidian::MouseScrolledEvent>([&](const Obsidian::MouseScrolledEvent& mse) { m_Project->OnEvent(Project::MouseScrolledEvent(mse)); });
            handler.Handle<Obsidian::MouseButtonPressedEvent>([&](const Obsidian::MouseButtonPressedEvent& mbpe) { m_Project->OnEvent(Project::MouseButtonPressedEvent(mbpe)); });
            handler.Handle<Obsidian::MouseButtonReleasedEvent>([&](const Obsidian::MouseButtonReleasedEvent& mbre) { m_Project->OnEvent(Project::MouseButtonReleasedEvent(mbre)); });
        }
    }

}