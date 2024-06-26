#pragma once

#pragma once

#include "Core/Core.h"

#include <functional>

namespace Ludu
{

    enum class EventType
    {
        None,
        WindowClose,
        WindowResize,
        KeyPress,
        KeyRelease,
        MouseButtonPress,
        MouseButtonRelease,
        MouseMove,
        MouseScroll
    };

    class Event
    {
    public:
        bool Handled = false;

        Event() = default;
        ~Event() = default;

        virtual EventType GetEventType() const = 0;
        virtual std::string ToString() const = 0;
    };

    inline std::ostream& operator<<(std::ostream& os, const Event& e)
    {
        return os << e.ToString();
    }
}