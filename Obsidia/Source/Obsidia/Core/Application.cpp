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
            m_Window.PollEvents();

            // Update
            {
                float currentTime = static_cast<float>(m_Window.GetWindowTime());
                float deltaTime = currentTime - lastTime;
                m_Specification.Project->OnUpdate(deltaTime);
                lastTime = static_cast<float>(currentTime);

                if (m_Specification.UpdateCallback) [[likely]]
                    m_Specification.UpdateCallback(deltaTime);
            }

            // Render
            {
                // TODO: Begin
                m_Specification.Project->OnRender();
                // TODO: End

                if (m_Specification.RenderCallback) [[likely]]
                    m_Specification.RenderCallback();
            }

            m_Window.SwapBuffers();
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Private methods
    ////////////////////////////////////////////////////////////////////////////////////
    void Application::OnEvent(Obsidian::Event e)
    {
        Nano::Events::EventHandler handler(e);
        handler.Handle<Obsidian::WindowCloseEvent>([&](Obsidian::WindowCloseEvent& wce) { (void)wce; m_Window.Close(); });
        //handler.Handle<Obsidian::WindowResizeEvent>([&](Obsidian::WindowResizeEvent& wre) { m_Window.Close(); }); // TODO: Resize renderer

        m_Specification.Project->OnEvent(e);

        if (m_Specification.EventCallback) [[likely]]
            m_Specification.EventCallback(e);
    }

}