#pragma once

#include "Platform/Vulkan/VulkanDevice.h"

#include <string>
#include <vector>

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
        VulkanPipeline(VulkanDevice& device, const std::string &vertFilepath, const std::string &fragFilepath, const PipelineConfigInfo& configInfo);
        ~VulkanPipeline();

        VulkanPipeline(const VulkanPipeline&) = delete;
        VulkanPipeline operator=(const VulkanPipeline&) = delete;

        void Bind(VkCommandBuffer commandBuffer);

        static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);

    private:
        VulkanDevice& m_Device;
        VkPipeline graphicsPipeline;
        VkShaderModule vertShaderModule;
        VkShaderModule fragShaderModule;

        static std::vector<char> readFile(const std::string &filepath);
        void createGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo);
        void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

    };
}