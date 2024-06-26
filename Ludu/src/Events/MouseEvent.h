#pragma once

#include <sstream>
#include "Core/MouseCodes.h"
#include "Events/Event.h"

namespace Ludu
{

    class MouseButtonEvent : public Event
    {
    public:
        MouseCode GetMouseButton() const { return m_Button; }

    protected:
        MouseCode m_Button;

        MouseButtonEvent(const MouseCode button)
            : m_Button(button) {}
    };

    class MouseButtonPressEvent : public MouseButtonEvent
    {
    public:
        MouseButtonPressEvent(const MouseCode button)
            : MouseButtonEvent(button) {}

        EventType GetEventType() const override { return EventType::MouseButtonPress; }
        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonPressEvent: " << m_Button;
            return ss.str();
        }
    };

    class MouseButtonReleaseEvent : public MouseButtonEvent
    {
    public:
        MouseButtonReleaseEvent(const MouseCode button)
            : MouseButtonEvent(button) {}

        EventType GetEventType() const override { return EventType::MouseButtonRelease; }
        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonReleaseEvent: " << m_Button;
            return ss.str();
        }
    };

    class MouseMoveEvent : public Event
    {
    public:
        MouseMoveEvent(const float x, const float y)
            : m_MouseX(x), m_MouseY(y) {}

        float GetX() const { return m_MouseX; }
        float GetY() const { return m_MouseY; }

        EventType GetEventType() const override { return EventType::MouseMove; }
        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseMoveEvent: " << m_MouseX << ", " << m_MouseY;
            return ss.str();
        }

    private:
        float m_MouseX, m_MouseY;
    };

    class MouseScrollEvent : public Event
    {
    public:
        MouseScrollEvent(const float xOffset, const float yOffset)
            : m_XOffset(xOffset), m_YOffset(yOffset) {}

        float GetXOffset() const { return m_XOffset; }
        float GetYOffset() const { return m_YOffset; }

        EventType GetEventType() const override { return EventType::MouseScroll; }
        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseScrollEvent: " << GetXOffset() << ", " << GetYOffset();
            return ss.str();
        }

    private:
        float m_XOffset, m_YOffset;
    };
}