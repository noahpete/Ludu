#pragma once

#include "Scene/Scene.h"

#include <entt/entt.hpp>

namespace Ludu
{

    class Entity
    {
    public:
        Entity()
            : m_UUID(Util::GenerateUUID()) {}
        Entity(entt::entity handle, Scene* scene)
            : m_UUID(Util::GenerateUUID()), m_EntityHandle(handle), m_Scene(scene) {}

        template<typename T, typename... Args>
        T& AddComponent(Args&&... args)
        {
            // LD_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
            return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
        }

        template<typename T>
        T& GetComponent()
        {
            // LD_CORE_ASSERT(HasComponent<T>(), "Entity doesn't have component!");
            return m_Scene->m_Registry.get<T>(m_EntityHandle);
        }

        template<typename T>
        bool HasComponent()
        {
            return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
        }

        template<typename T>
        void RemoveComponent()
        {
            LD_CORE_ASSERT(HasComponent<T>(), "Entity doesn't have component!");
            m_Scene->m_Registry.remove<T>(m_EntityHandle);
        }

        uint32_t GetUUID() const { return m_UUID; }

        operator bool() const { return m_EntityHandle != entt::null; }
        operator uint32_t() const { return (uint32_t)m_EntityHandle; }
        operator entt::entity() const { return m_EntityHandle; }

        bool operator==(const Entity& other) const { return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene; }
        bool operator!=(const Entity& other) const { return !operator==(other); }

    private:
        uint64_t m_UUID;
        entt::entity m_EntityHandle{ entt::null };
        Scene* m_Scene;

    };
}