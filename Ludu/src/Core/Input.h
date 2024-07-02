#pragma once

#include "Core/KeyCodes.h"
#include "Core/MouseCodes.h"

#include <glm/glm.hpp>

namespace Ludu {

    class Input
    {
    public:
        static bool IsKeyPressed(KeyCode key) { return s_Instance->IsKeyPressedImpl(key); }

        static bool IsMouseButtonPressed(MouseCode button) { return s_Instance->IsMouseButtonPressedImpl(button); }
        static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
        static float GetMouseY() { return s_Instance->GetMouseYImpl(); }
        static glm::vec2 GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
        // static glm::vec2 GetMouseScroll() { return s_Instance->GetMouseScrollImpl(); }

    protected:
        virtual bool IsKeyPressedImpl(KeyCode key) = 0;
        virtual bool IsMouseButtonPressedImpl(MouseCode button) = 0;
        virtual float GetMouseXImpl() = 0;
        virtual float GetMouseYImpl() = 0;
        virtual glm::vec2 GetMousePositionImpl() = 0;
        // virtual glm::vec2 GetMouseScrollImpl() = 0;

    private:
        static Input* s_Instance;

    };
}