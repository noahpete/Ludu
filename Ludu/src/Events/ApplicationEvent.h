#pragma once

#include <sstream>
#include "Event.h"

namespace Ludu
{
    class WindowResizeEvent : public Event
    {
    public:
        uint32_t m_Width, m_Height;

        WindowResizeEvent(uint32_t newWidth, uint32_t newHeight)
            : m_Width(newWidth), m_Height(newHeight) {}
        EventType GetEventType() const override { return EventType::WindowResize; }
        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "WindowResizeEvent";
            return ss.str();
        }

    };

    class QuitEvent : public Event
    {
    public:
        QuitEvent() = default;
        EventType GetEventType() const override { return EventType::WindowClose; }
        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "QuitEvent";
            return ss.str();
        }
    };
}