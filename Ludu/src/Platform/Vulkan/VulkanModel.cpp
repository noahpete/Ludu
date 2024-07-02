#include "ldpch.h"
#include "VulkanModel.h"

namespace Ludu
{

    VulkanModel::VulkanModel(VulkanDevice &device, const std::vector<Vertex> &vertices)
        : m_Device{device}
    {
        CreateVertexBuffers(vertices);
    }

    VulkanModel::~VulkanModel()
    {
        vkDestroyBuffer(m_Device.device(), m_VertexBuffer, nullptr);
        vkFreeMemory(m_Device.device(), m_VertexBufferMemory, nullptr);
    }

    void VulkanModel::Bind(VkCommandBuffer commandBuffer)
    {
        VkBuffer buffers[] = { m_VertexBuffer };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
    }

    void VulkanModel::Draw(VkCommandBuffer commandBuffer)
    {
        vkCmdDraw(commandBuffer, m_VertexCount, 1, 0, 0);
    }

    void VulkanModel::CreateVertexBuffers(const std::vector<Vertex> &vertices)
    {
        m_VertexCount = static_cast<uint32_t>(vertices.size());
        VkDeviceSize bufferSize = sizeof(vertices[0]) * m_VertexCount;
        m_Device.createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_VertexBuffer, m_VertexBufferMemory);

        void *data;
        vkMapMemory(m_Device.device(), m_VertexBufferMemory, 0, bufferSize, 0, &data);

        memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
        vkUnmapMemory(m_Device.device(), m_VertexBufferMemory);
    }

    std::vector<VkVertexInputBindingDescription> Vertex::GetBindingDescriptions()
    {
        return { {0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX} };
    }

    std::vector<VkVertexInputAttributeDescription> Vertex::GetAttributeDescriptions()
    {
        return { {0, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, position) },
                 {1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color)} }; // location, binding, format, offset
    }
}