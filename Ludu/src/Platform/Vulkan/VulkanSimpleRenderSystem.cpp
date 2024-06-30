#include "Platform/Vulkan/VulkanSimpleRenderSystem.h"

namespace Ludu {

	VulkanSimpleRenderSystem::VulkanSimpleRenderSystem(VulkanDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout)
        : m_Device{device}
	{
		CreatePipelineLayout(globalSetLayout);
		CreatePipeline(renderPass);
	}

	VulkanSimpleRenderSystem::~VulkanSimpleRenderSystem()
	{
		vkDestroyPipelineLayout(m_Device.device(), m_PipelineLayout, nullptr);
	}

    void VulkanSimpleRenderSystem::CreatePipelineLayout(VkDescriptorSetLayout globalSetLayout)
    {
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(SimplePushConstantData);

		std::vector<VkDescriptorSetLayout> descriptorSetLayouts{ globalSetLayout };

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
		pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

		if (vkCreatePipelineLayout(m_Device.device(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS)
			LD_CORE_ERROR("Failed to create pipeline layout!");
    }

    void VulkanSimpleRenderSystem::CreatePipeline(VkRenderPass renderPass)
    {
		LD_CORE_ASSERT(m_PipelineLayout != nullptr, "Cannot create pipeline before pipeline layout!");

		PipelineConfigInfo pipelineConfig{};
		VulkanPipeline::defaultPipelineConfigInfo(pipelineConfig);
		pipelineConfig.RenderPass = renderPass;
		pipelineConfig.PipelineLayout = m_PipelineLayout;
		m_Pipeline = std::make_unique<VulkanPipeline>(m_Device, "simple_shader.vert.spv", "simple_shader.frag.spv", pipelineConfig);
    }

    void VulkanSimpleRenderSystem::RenderGameObjects(FrameInfo& frameInfo, std::vector<VulkanGameObject> &gameObjects)
    {
		m_Pipeline->Bind(frameInfo.commandBuffer);

		vkCmdBindDescriptorSets(frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_PipelineLayout, 0, 1, &frameInfo.globalDescriptorSet, 0, nullptr);

		for (auto& obj : gameObjects)
		{
			SimplePushConstantData push{};
			
			push.ModelMatrix = obj.transform.mat4();
			push.NormalMatrix = obj.transform.normalMatrix();

			vkCmdPushConstants(frameInfo.commandBuffer, m_PipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);

			obj.model->Bind(frameInfo.commandBuffer);
			obj.model->Draw(frameInfo.commandBuffer);
		}
    }
}
