#include "rppch.h"
#include "Window.hpp"

#include "Rapid/Core/Core.hpp"
#include "Rapid/Core/Logger.hpp"

#include "Rapid/Project/Events.hpp"

namespace Rapid
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Constructor & Destructor
    ////////////////////////////////////////////////////////////////////////////////////
    Window::Window(const WindowSpecification& specs)
        : m_Specification(specs)
        , m_Window(Obsidian::WindowSpecification()
            .SetTitle(specs.Title)
            .SetWidthAndHeight(specs.Width, specs.Height)
            .SetFlags(Obsidian::WindowFlags::Default)
            .SetEventCallback([this](Obsidian::Event e) { OnEvent(e); })
        )
        , m_Renderer(m_Window, specs.VSync)
    {
        RP_ASSERT(m_Specification.EventCallback, "Must have an event callback.");
    }

    Window::~Window()
    {
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Methods
    ////////////////////////////////////////////////////////////////////////////////////
    void Window::SwapBuffers()
    {
        m_Window.SwapBuffers();
        m_Renderer.End();
    }

    void Window::PollEvents()
    {
        m_Window.PollEvents();
        m_Renderer.Begin();
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Private methods
    ////////////////////////////////////////////////////////////////////////////////////
    void Window::OnEvent(Obsidian::Event e)
    {
        Nano::Events::EventHandler handler(e);

        handler.Handle<Obsidian::WindowCloseEvent>([&](const Obsidian::WindowCloseEvent& wce) { (void)wce; m_Window.Close();  });
        handler.Handle<Obsidian::WindowResizeEvent>([&](const Obsidian::WindowResizeEvent& wre) { m_Renderer.Resize(wre.GetWidth(), wre.GetHeight()); });

        m_Specification.EventCallback(e);
    }

}