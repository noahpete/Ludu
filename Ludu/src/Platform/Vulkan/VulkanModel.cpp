#include "ldpch.h"
#include "Platform/Vulkan/VulkanModel.h"

#include "Core/Core.h"

namespace Ludu {
	
	VulkanModel::VulkanModel(VulkanDevice& device, const std::vector<Vertex>& vertices)
		: m_Device{device}
	{
		CreateVertexBuffers(vertices);
	}

	VulkanModel::~VulkanModel()
	{
		vkDestroyBuffer(m_Device.device(), vertexBuffer, nullptr);
		vkFreeMemory(m_Device.device(), vertexBufferMemory, nullptr);
	}

	void VulkanModel::Bind(VkCommandBuffer commandBuffer)
	{
		VkBuffer buffers[] = { vertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
	}

	void VulkanModel::Draw(VkCommandBuffer commandBuffer)
	{
		vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
	}

	void VulkanModel::CreateVertexBuffers(const std::vector<Vertex>& vertices)
	{
		vertexCount = static_cast<uint32_t>(vertices.size());
		LD_CORE_ASSERT(vertexCount >= 3, "Vertex count must be at least 3!");

		VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
		m_Device.createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vertexBuffer, vertexBufferMemory);

		void* data;
		vkMapMemory(m_Device.device(), vertexBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
		vkUnmapMemory(m_Device.device(), vertexBufferMemory);
	}

	std::vector<VkVertexInputBindingDescription> VulkanModel::Vertex::GetBindingDescriptions()
	{
		std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);

		bindingDescriptions[0].binding = 0;
		bindingDescriptions[0].stride = sizeof(Vertex);
		bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescriptions;
	}

	std::vector<VkVertexInputAttributeDescription> VulkanModel::Vertex::GetAttributeDescriptions()
	{
		std::vector<VkVertexInputAttributeDescription> attrDescriptions(2);

		attrDescriptions[0].binding = 0;
		attrDescriptions[0].location = 0;
		attrDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		attrDescriptions[0].offset = offsetof(Vertex, position);

		attrDescriptions[1].binding = 0;
		attrDescriptions[1].location = 1;
		attrDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attrDescriptions[1].offset = offsetof(Vertex, color);

		return attrDescriptions;
	}
}