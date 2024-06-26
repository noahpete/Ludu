#pragma once

#include <sstream>
#include "Event.h"

namespace Ludu
{

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