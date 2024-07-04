#pragma once

#include "Platform/Vulkan/VulkanDevice.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>

namespace Ludu
{
    struct Vertex {
        glm::vec3 position;
        glm::vec3 color;

        static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions();
        static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();

    };

    class VulkanModel
    {
    public:
        VulkanModel(const std::vector<Vertex> &vertices);
        ~VulkanModel();

        VulkanModel(const VulkanModel &) = delete;
        VulkanModel &operator=(const VulkanModel &) = delete;

        void Bind(VkCommandBuffer commandBuffer);
        void Draw(VkCommandBuffer commandBuffer);

    private:
        VulkanDevice& m_Device;
        VkBuffer m_VertexBuffer;
        VkDeviceMemory m_VertexBufferMemory;
        uint32_t m_VertexCount;

        void CreateVertexBuffers(const std::vector<Vertex> &vertices);
    };
}