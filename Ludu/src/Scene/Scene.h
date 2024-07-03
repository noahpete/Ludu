#pragma once

#include "Core/Timestep.h"
//#include "Renderer/Camera.h"

#include <entt/entt.hpp>

namespace Ludu
{
    class Entity;

    class Scene
    {
        friend class Entity;
        friend class SceneHierarchyPanel;

    public:
        Scene();
        ~Scene();

        Entity CreateEntity(const std::string& name);
        void DestroyEntity(Entity entity);

        void OnUpdate(Timestep ts);

    private:
        entt::registry m_Registry;

    };
}