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

        glm::mat4 mat4()
        {
            const float c3 = glm::cos(Rotation.z);
            const float s3 = glm::sin(Rotation.z);
            const float c2 = glm::cos(Rotation.x);
            const float s2 = glm::sin(Rotation.x);
            const float c1 = glm::cos(Rotation.y);
            const float s1 = glm::sin(Rotation.y);
            return glm::mat4{
                {
                    Scale.x * (c1 * c3 + s1 * s2 * s3),
                    Scale.x * (c2 * s3),
                    Scale.x * (c1 * s2 * s3 - c3 * s1),
                    0.0f,
                },
                {
                    Scale.y * (c3 * s1 * s2 - c1 * s3),
                    Scale.y * (c2 * c3),
                    Scale.y * (c1 * c3 * s2 + s1 * s3),
                    0.0f,
                },
                {
                    Scale.z * (c2 * s1),
                    Scale.z * (-s2),
                    Scale.z * (c1 * c2),
                    0.0f,
                },
                {Translation.x, Translation.y, Translation.z, 1.0f}};
        }
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