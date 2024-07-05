#include "ldpch.h"
#include "EditorLayer.h"

#include "Core/Input.h"

namespace Ludu {

    EditorLayer::EditorLayer()
    {
    }

    EditorLayer::~EditorLayer()
    {
    }

    void EditorLayer::OnAttach()
    {
        m_Camera = CreateRef<Camera>();
    }

    void EditorLayer::OnDetach()
    {
    }

    void EditorLayer::OnUpdate(Timestep ts)
    {
        auto& pos = m_Camera->GetPosition();
        auto& rot = m_Camera->GetRotation();

        if (Input::IsKeyPressed(KeyCode::W))
        {
            pos.z += 0.05f;
        }
        else if (Input::IsKeyPressed(KeyCode::S))
        {
            pos.z -= 0.05f;
        }

        if (Input::IsKeyPressed(KeyCode::A))
        {
            pos.x -= 0.05f;
        }
        else if (Input::IsKeyPressed(KeyCode::D))
        {
            pos.x += 0.05f;
        }

        rot.x = glm::clamp(rot.x, -1.5f, 1.5f);
        rot.y = glm::mod(rot.y, 6.28f);

        m_Camera->SetViewYXZ(pos, rot);
    }
}