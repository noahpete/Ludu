#include "ldpch.h"
#include "Scene.h"
#include "Scene/Components.h"

//#include "Platform/SDL2/SDL2Renderer.h"
#include "Core/Application.h"
#include "Scene/Entity.h"
#include <glm/glm.hpp>
#include "Scene.h"

namespace Ludu
{

    Scene::Scene()
    {
        // entt::entity entity = m_Registry.create();

        // m_Registry.emplace<TransformComponent>(entity);
    }

    Scene::~Scene()
    {
    }

    Entity Scene::CreateEntity(const std::string& name = "")
    {
        Entity entity = { m_Registry.create(), this };
        entity.AddComponent<TransformComponent>();
        auto& tag = entity.AddComponent<TagComponent>();
        tag.Tag = name.empty() ? "Entity" : name;
        return entity;
    }

    void Scene::DestroyEntity(Entity entity)
    {
        m_Registry.destroy(entity);
    }

    void Scene::OnUpdate(Timestep ts)
    {
        // Update scripts
        //m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
        //    {
        //        // TODO: Move to Scene::OnScenePlay
        //        if (!nsc.Instance)
        //        {
        //            nsc.Instance = nsc.InstantiateScript();
        //            nsc.Instance->m_Entity = Entity{ entity, this };
        //            nsc.Instance->OnCreate();
        //        }

        //        nsc.Instance->OnUpdate(ts);
        //    });

        // Render stuff
        /*auto renderer = Application::Get().GetRenderer();

        auto view = m_Registry.view<CameraComponent, TransformComponent>();
        for (auto entity : view)
        {
            if (view.get<CameraComponent>(entity).Primary)
            {
                m_Camera = view.get<CameraComponent>(entity).Camera;
                renderer->SetCamera(&m_Camera);

                auto& tc = view.get<TransformComponent>(entity);
                renderer->SetCameraTransform(&tc);
            }
        }

        auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        for (auto entity : group)
        {
            auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
            SDL2Renderer::SubmitPrimitiveRequest(transform.Position, { 50, 50 }, transform.Rotation, sprite.Color, 0);
        }*/
    }
}