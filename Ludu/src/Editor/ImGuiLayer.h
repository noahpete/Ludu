#pragma once

#include "Core/Layer.h"
#include "Events/EventManager.h"

namespace Ludu
{

    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer() = default;

        void OnAttach() override;
        void OnDetach() override;
        void OnEvent(Event& e) override;

        void Begin();
        void End();


    };
}