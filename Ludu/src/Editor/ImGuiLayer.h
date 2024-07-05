#pragma once

#include "Core/Layer.h"
#include "Events/EventManager.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace Ludu
{

    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer() = default;

        void OnAttach() override;
        void OnDetach() override;

        void Begin();
        void End();


    };
}