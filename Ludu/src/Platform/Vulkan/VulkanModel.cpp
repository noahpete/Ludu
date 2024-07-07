#include "VulkanModel.h"
#include "ldpch.h"
#include "VulkanModel.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader/tiny_obj_loader.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

namespace std {

    template<>
    struct hash<Ludu::Vertex>
    {
        size_t operator()(Ludu::Vertex const& vertex) const
        {
            size_t seed = 0;
            Util::HashCombine(seed, vertex.position, vertex.color, vertex.Normal, vertex.UV);
            return seed;
        }
    };
}
namespace Ludu
{

    VulkanModel::VulkanModel(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices)
        : m_Device{VulkanDevice::Get()}
    {
        CreateVertexBuffers(vertices);
        CreateIndexBuffers(indices);
        m_Device.addModel(this);
    }

    VulkanModel::~VulkanModel()
    {
    }

    void VulkanModel::Bind(VkCommandBuffer commandBuffer)
    {
        VkBuffer buffers[] = { m_VertexBuffer->getBuffer()};
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

        if (m_HasIndexBuffer)
        {
            vkCmdBindIndexBuffer(commandBuffer, m_IndexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
        }
    }

    void VulkanModel::Draw(VkCommandBuffer commandBuffer)
    {
        if (m_HasIndexBuffer)
        {
            vkCmdDrawIndexed(commandBuffer, m_IndexCount, 1, 0, 0, 0);
        }
        else
        {
            vkCmdDraw(commandBuffer, m_VertexCount, 1, 0, 0);
        }
    }

    Scope<VulkanModel> VulkanModel::CreateModelFromFile(const std::string& filepath)
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str()))
            LD_CORE_ERROR("Warning or error while loading .obj file: {0}; {1}", warn, err);

        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        std::unordered_map<Vertex, uint32_t> uniqueVertices{};

        for (const auto& shape : shapes)
        {
            for (const auto& index : shape.mesh.indices)
            {
                Vertex vertex{};

                if (index.vertex_index >= 0)
                {
                    vertex.position = {
                        attrib.vertices[3 * index.vertex_index + 0],
                        attrib.vertices[3 * index.vertex_index + 1],
                        attrib.vertices[3 * index.vertex_index + 2]
                    };

                    vertex.color = {
                        attrib.colors[3 * index.vertex_index + 0],
                        attrib.colors[3 * index.vertex_index + 1],
                        attrib.colors[3 * index.vertex_index + 2]
                    };
                }

                if (index.normal_index >= 0)
                {
                    vertex.Normal = {
                        attrib.normals[3 * index.normal_index + 0],
                        attrib.normals[3 * index.normal_index + 1],
                        attrib.normals[3 * index.normal_index + 2]
                    };
                }

                if (index.texcoord_index >= 0)
                {
                    vertex.UV = {
                        attrib.texcoords[2 * index.texcoord_index + 0],
                        attrib.texcoords[2 * index.texcoord_index + 1]
                    };
                }

                if (uniqueVertices.count(vertex) == 0)
                {
                    uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex);
                }
                indices.push_back(uniqueVertices[vertex]);
            }
        }

        return CreateScope<VulkanModel>(vertices, indices);
    }

    void VulkanModel::CreateVertexBuffers(const std::vector<Vertex> &vertices)
    {
        m_VertexCount = static_cast<uint32_t>(vertices.size());

        VkDeviceSize bufferSize = sizeof(vertices[0]) * m_VertexCount;
        uint32_t vertexSize = sizeof(vertices[0]);

        VulkanBuffer stagingBuffer{
            m_Device,
            vertexSize,
            m_VertexCount,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        };

        stagingBuffer.map();
        stagingBuffer.writeToBuffer((void*)vertices.data());

        m_VertexBuffer = CreateScope<VulkanBuffer>(
            m_Device,
            vertexSize,
            m_VertexCount,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        m_Device.copyBuffer(stagingBuffer.getBuffer(), m_VertexBuffer->getBuffer(), bufferSize);
    }

    void VulkanModel::CreateIndexBuffers(const std::vector<uint32_t>& indices)
    {
        m_IndexCount = static_cast<uint32_t>(indices.size());
        m_HasIndexBuffer = m_IndexCount > 0;

        if (!m_HasIndexBuffer)
            return;

        VkDeviceSize bufferSize = sizeof(indices[0]) * m_IndexCount;
        uint32_t indexSize = sizeof(indices[0]);

        VulkanBuffer stagingBuffer{
            m_Device,
            indexSize,
            m_IndexCount,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        };

        stagingBuffer.map();
        stagingBuffer.writeToBuffer((void*)indices.data());

        m_IndexBuffer = CreateScope<VulkanBuffer>(
            m_Device,
            indexSize,
            m_IndexCount,
            VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        m_Device.copyBuffer(stagingBuffer.getBuffer(), m_IndexBuffer->getBuffer(), bufferSize);
    }

    std::vector<VkVertexInputBindingDescription> Vertex::GetBindingDescriptions()
    {
        return { {0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX} };
    }

    std::vector<VkVertexInputAttributeDescription> Vertex::GetAttributeDescriptions()
    {
        std::vector<VkVertexInputAttributeDescription> attrDescriptions{};
        attrDescriptions.push_back({ 0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position) });
        attrDescriptions.push_back({ 1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color) });
        attrDescriptions.push_back({ 2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, Normal) });
        attrDescriptions.push_back({ 3, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, UV) });
        return attrDescriptions;
    }
}