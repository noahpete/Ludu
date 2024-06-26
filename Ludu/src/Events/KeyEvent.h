#pragma once

#include <sstream>
#include "Core/KeyCodes.h"
#include "Events/Event.h"

namespace Ludu
{

    class KeyEvent : public Event
    {
    public:
        KeyCode GetKeyCode() const { return key; }

    protected:
        KeyCode key;

        KeyEvent(const KeyCode keycode)
            : key(keycode) {}
    };

    class KeyPressEvent : public KeyEvent
    {
    public:
        bool IsRepeat;

        KeyPressEvent(const KeyCode keycode, bool isRepeat = false)
            : KeyEvent(keycode), IsRepeat(isRepeat) {}

        EventType GetEventType() const override { return EventType::KeyPress; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyPressEvent: " << key << " (repeat = " << IsRepeat << ")";
            return ss.str();
        }
    };

    class KeyReleaseEvent : public KeyEvent
    {
    public:
        KeyReleaseEvent(const KeyCode keycode)
            : KeyEvent(keycode) {}

        EventType GetEventType() const override { return EventType::KeyRelease; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyReleaseEvent: " << key;
            return ss.str();
        }
    };
}