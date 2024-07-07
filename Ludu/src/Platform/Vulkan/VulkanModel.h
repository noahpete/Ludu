#pragma once

#include "Platform/Vulkan/VulkanDevice.h"
#include "Platform/Vulkan/VulkanBuffer.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>

namespace Ludu
{
    struct Vertex
    {
        glm::vec3 position{};
        glm::vec3 color{};
        glm::vec3 Normal{};
        glm::vec2 UV{};

        static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions();
        static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();

        bool operator==(const Vertex& other) const { return position == other.position && color == other.color && Normal == other.Normal && UV == other.UV;  }
    };

    class VulkanModel
    {
    public:
        VulkanModel(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices);
        ~VulkanModel();

        VulkanModel(const VulkanModel &) = delete;
        VulkanModel &operator=(const VulkanModel &) = delete;

        void Bind(VkCommandBuffer commandBuffer);
        void Draw(VkCommandBuffer commandBuffer);

        static Scope<VulkanModel> CreateModelFromFile(const std::string& filepath);

    private:
        VulkanDevice& m_Device;

        Scope<VulkanBuffer> m_VertexBuffer;
        uint32_t m_VertexCount;

        bool m_HasIndexBuffer = false;
        Scope<VulkanBuffer> m_IndexBuffer;
        uint32_t m_IndexCount;

        void CreateVertexBuffers(const std::vector<Vertex>& vertices);
        void CreateIndexBuffers(const std::vector<uint32_t> &indices);
    };
}