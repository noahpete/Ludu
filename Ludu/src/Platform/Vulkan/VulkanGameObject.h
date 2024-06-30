#pragma once

#include "Platform/Vulkan/VulkanModel.h"

#include <glm/gtc/matrix_transform.hpp>

#include <memory>

namespace Ludu
{

    struct TransformComponent
    {
        glm::vec3 Translation{};
        glm::vec3 Scale{1.0f};
        glm::vec3 Rotation{};

        glm::mat4 mat4();
        glm::mat3 normalMatrix();
    };

    class VulkanGameObject
    {
    public:
        std::shared_ptr<VulkanModel> model{};
        glm::vec3 color{};
        TransformComponent transform{};

        static VulkanGameObject CreateGameObject()
        {
            static uint32_t currentID = 0;
            return VulkanGameObject{currentID++};
        }

        VulkanGameObject(const VulkanGameObject &) = delete;
        VulkanGameObject &operator=(const VulkanGameObject &) = delete;
        VulkanGameObject(VulkanGameObject &&) = default;
        VulkanGameObject &operator=(VulkanGameObject &&) = default;

        uint32_t GetID() { return m_ID; }

    private:
        uint32_t m_ID;

        VulkanGameObject(uint32_t objID) : m_ID(objID) {}
    };
}