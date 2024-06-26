#pragma once

#include "Platform/Vulkan/VulkanDevice.h"

namespace Ludu {

	struct PipelineConfigInfo
	{
		VkViewport Viewport;
		VkRect2D Scissor;
		VkPipelineInputAssemblyStateCreateInfo InputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo RasterizationInfo;
		VkPipelineMultisampleStateCreateInfo MultisampleInfo;
		VkPipelineColorBlendAttachmentState ColorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo ColorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo DepthStencilInfo;
		VkPipelineLayout PipelineLayout = nullptr;
		VkRenderPass RenderPass = nullptr;
		uint32_t Subpass = 0;
	};

	class VulkanPipeline {
	public:
		VulkanPipeline(VulkanDevice& device, const PipelineConfigInfo& configInfo, const std::string& vertFilepath, const std::string& fragFilepath);
		~VulkanPipeline();

		VulkanPipeline(const VulkanPipeline&) = delete;
		void operator=(const VulkanPipeline&) = delete;

		static PipelineConfigInfo DefaultPipelineConfigInfo(uint32_t width, uint32_t height);

	private:
		VulkanDevice& m_Device;
		VkPipeline m_GraphicsPipeline;
		VkShaderModule m_VertShaderModule;
		VkShaderModule m_FragShaderModule;

		static std::vector<char> ReadFile(const std::string& filepath);
		void CreateGraphicsPipeline(const PipelineConfigInfo& configInfo, const std::string& vertFilepath, const std::string& fragFilepath);
		void CreateShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

	};
}