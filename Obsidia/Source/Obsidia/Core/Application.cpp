#include "obpch.h"
#include "Application.hpp"

#include "Obsidia/Core/Core.hpp"
#include "Obsidia/Core/Logger.hpp"

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
    {
        OBSIDIA_ASSERT(specs.Project, "[Application] To create an application a project must be specified.");
    }

    Application::~Application()
    {
        m_Specification.Project.reset();
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
                m_Specification.Project->OnUpdate(deltaTime);
                lastTime = static_cast<float>(currentTime);
            }

            // Render
            {
                m_Renderer->Begin();
                m_Specification.Project->OnRender();
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
        handler.Handle<Obsidian::WindowCloseEvent>([&](Obsidian::WindowCloseEvent& wce) 
        { 
            (void)wce; 
            m_Window.Close(); 
            m_Renderer.Destroy();
        });
        handler.Handle<Obsidian::WindowResizeEvent>([&](Obsidian::WindowResizeEvent& wre) { m_Renderer->Resize(wre.GetWidth(), wre.GetHeight()); });

        m_Specification.Project->OnEvent(e);
    }

}