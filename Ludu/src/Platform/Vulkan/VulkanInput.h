#pragma once

#include "Core/Input.h"
#include "Events/EventManager.h"

#include <GLFW/glfw3.h>

namespace Ludu
{
    class VulkanInput : public Input
    {
    protected:
        bool IsKeyPressedImpl(KeyCode key) override;
        bool IsMouseButtonPressedImpl(MouseCode button) override;
        float GetMouseXImpl() override;
        float GetMouseYImpl() override;
        glm::vec2 GetMousePositionImpl() override;
        // glm::vec2 GetMouseScrollImpl() override;
    };
}