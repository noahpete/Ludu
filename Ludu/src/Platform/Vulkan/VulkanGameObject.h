#pragma once

#include "Platform/Vulkan/VulkanModel.h"

#include <memory>

namespace Ludu
{

    struct Transform2DComponent
    {
        glm::vec2 Translation{};
        glm::vec2 Scale{1.0f};
        float rotation;

        glm::mat2 mat2()
        {
            const float s = glm::sin(rotation);
            const float c = glm::cos(rotation);
            glm::mat2 rotationMatrix{{c, s}, {-s, c}};
            glm::mat2 scaleMatrix{{Scale.x, 0.0f}, {0.0f, Scale.y}};
            return rotationMatrix * scaleMatrix;
        }
    };

    class VulkanGameObject
    {
    public:
        std::shared_ptr<VulkanModel> model{};
        glm::vec3 color{};
        Transform2DComponent transform2D{};

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