#pragma once

#include "Rapid/Core/Core.hpp"

#include <Obsidian/Core/Input/KeyCodes.hpp>
#include <Obsidian/Core/Input/MouseCodes.hpp>

#include <Obsidian/Core/Events.hpp>

#include <cstdint>

namespace Rapid::Project
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Window events
    ////////////////////////////////////////////////////////////////////////////////////
    class ResizeEvent
    {
    public:
        // Constructors & Destructor
        inline ResizeEvent(uint32_t width, uint32_t height)
            : m_Width(width), m_Height(height) {
        }
        inline ResizeEvent(const Obsidian::WindowResizeEvent& wre)
            : m_Width(wre.GetWidth()), m_Height(wre.GetHeight()) {
        }
        ~ResizeEvent() = default;

        // Getters
        inline uint32_t GetWidth() const { return m_Width; }
        inline uint32_t GetHeight() const { return m_Height; }

    private:
        uint32_t m_Width, m_Height;
    };

    ////////////////////////////////////////////////////////////////////////////////////
    // Key events
    ////////////////////////////////////////////////////////////////////////////////////
    class KeyPressedEvent
    {
    public:
        // Constructors & Destructor
        inline KeyPressedEvent(Obsidian::Key key, int repeatCount)
            : m_KeyCode(key), m_RepeatCount(repeatCount) {
        }
        inline KeyPressedEvent(const Obsidian::KeyPressedEvent& kpe)
            : m_KeyCode(kpe.GetKeyCode()), m_RepeatCount(kpe.GetRepeatCount()) {
        }
        ~KeyPressedEvent() = default;

        // Getters
        inline Obsidian::Key GetKeyCode() const { return m_KeyCode; }

    private:
        Obsidian::Key m_KeyCode;
        int m_RepeatCount;
    };

    class KeyReleasedEvent
    {
    public:
        // Constructors & Destructor
        inline KeyReleasedEvent(Obsidian::Key key)
            : m_KeyCode(key) {
        }
        inline KeyReleasedEvent(const Obsidian::KeyReleasedEvent& kre)
            : m_KeyCode(kre.GetKeyCode()) {
        }
        ~KeyReleasedEvent() = default;

        // Getters
        inline Obsidian::Key GetKeyCode() const { return m_KeyCode; }

    private:
        Obsidian::Key m_KeyCode;
    };

    ////////////////////////////////////////////////////////////////////////////////////
    // Mouse events
    ////////////////////////////////////////////////////////////////////////////////////
    class MouseMovedEvent
    {
    public:
        // Constructors & Destructor
        inline MouseMovedEvent(float x, float y)
            : m_MouseX(x), m_MouseY(y) {
        }
        inline MouseMovedEvent(const Obsidian::MouseMovedEvent& mme)
            : m_MouseX(mme.GetX()), m_MouseY(mme.GetY()) {
        }
        ~MouseMovedEvent() = default;

        // Getters
        inline float GetX() const { return m_MouseX; }
        inline float GetY() const { return m_MouseY; }

    private:
        float m_MouseX, m_MouseY;
    };

    class MouseScrolledEvent
    {
    public:
        // Constructors & Destructor
        inline MouseScrolledEvent(float xOffset, float yOffset)
            : m_XOffset(xOffset), m_YOffset(yOffset) {
        }
        inline MouseScrolledEvent(const Obsidian::MouseScrolledEvent& mse)
            : m_XOffset(mse.GetXOffset()), m_YOffset(mse.GetYOffset()) {
        }
        ~MouseScrolledEvent() = default;

        // Getters
        inline float GetXOffset() const { return m_XOffset; }
        inline float GetYOffset() const { return m_YOffset; }

    private:
        float m_XOffset, m_YOffset;
    };

    class MouseButtonPressedEvent
    {
    public:
        // Constructors & Destructor
        inline MouseButtonPressedEvent(Obsidian::MouseButton button)
            : m_Button(button) {
        }
        inline MouseButtonPressedEvent(const Obsidian::MouseButtonPressedEvent& mbpe)
            : m_Button(mbpe.GetButtonCode()) {
        }
        ~MouseButtonPressedEvent() = default;

        // Getters
        inline Obsidian::MouseButton GetButtonCode() const { return m_Button; }

    private:
        Obsidian::MouseButton m_Button;
    };

    class MouseButtonReleasedEvent
    {
    public:
        // Constructors & Destructor
        inline MouseButtonReleasedEvent(Obsidian::MouseButton button)
            : m_Button(button) {
        }
        inline MouseButtonReleasedEvent(const Obsidian::MouseButtonReleasedEvent& mbre)
            : m_Button(mbre.GetButtonCode()) {
        }
        ~MouseButtonReleasedEvent() = default;

        // Getters
        inline Obsidian::MouseButton GetButtonCode() const { return m_Button; }

    private:
        Obsidian::MouseButton m_Button;
    };

    ////////////////////////////////////////////////////////////////////////////////////
    // Event
    ////////////////////////////////////////////////////////////////////////////////////
    using Event = std::variant<
        ResizeEvent,
        KeyPressedEvent, KeyReleasedEvent,
        MouseMovedEvent, MouseScrolledEvent, MouseButtonPressedEvent, MouseButtonReleasedEvent
    >;

}
