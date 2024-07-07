#pragma once

#include "Core/Layer.h"

#include "Renderer/Camera.h"

namespace Ludu
{
    class EditorLayer : public Layer
    {
    public:
        EditorLayer();
        ~EditorLayer();

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(Timestep ts) override;
        void OnImGuiRender() override;

        Ref<Camera> GetEditorCamera() const { return m_Camera; }

    private:
        Ref<Camera> m_Camera;

    };
}