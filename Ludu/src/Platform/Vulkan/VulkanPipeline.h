#pragma once

#include "Core/Core.h"
#include "Platform/Vulkan/VulkanDevice.h"

namespace Ludu
{
    struct PipelineConfigInfo
    {
        VkPipelineViewportStateCreateInfo ViewportInfo;
        VkPipelineInputAssemblyStateCreateInfo InputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo RasterizationInfo;
        VkPipelineMultisampleStateCreateInfo MultisampleInfo;
        VkPipelineColorBlendAttachmentState ColorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo ColorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo DepthStencilInfo;
        std::vector<VkDynamicState> DynamicStateEnables;
        VkPipelineDynamicStateCreateInfo DynamicStateInfo;
        VkPipelineLayout PipelineLayout = nullptr;
        VkRenderPass RenderPass = nullptr;
        uint32_t Subpass = 0;
    };

    class VulkanPipeline
    {
    public:
        const std::string VERT_FILEPATH = "../simple_shader.vert.spv";
        const std::string FRAG_FILEPATH = "../simple_shader.frag.spv";

        VulkanPipeline(VulkanDevice &device, const PipelineConfigInfo &configInfo);
        ~VulkanPipeline();

        VulkanPipeline(const VulkanPipeline&) = delete;
        VulkanPipeline operator=(const VulkanPipeline&) = delete;

        void Bind(VkCommandBuffer commandBuffer);

        void CreateGraphicsPipeline(const PipelineConfigInfo &configInfo);
        void CreateShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

        static void DefaultPipelineConfigInfo(PipelineConfigInfo& configInfo);

    private:
        VulkanDevice& m_Device;
        VkPipeline m_GraphicsPipeline;
        VkShaderModule m_VertShaderModule;
        VkShaderModule m_FragShaderModule;

    };
}