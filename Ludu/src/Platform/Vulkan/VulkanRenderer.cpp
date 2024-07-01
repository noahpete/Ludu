#include "ldpch.h"
#include "VulkanRenderer.h"

namespace Ludu
{

    VulkanRenderer::VulkanRenderer(Ref<VulkanWindow> window)
        : m_Device{*window},
          m_SwapChain{m_Device, window->GetExtent()},
          m_Pipeline{}
    {
        CreatePipelineLayout();
        CreatePipeline();
        CreateCommandBuffers();
    }

    VulkanRenderer::~VulkanRenderer()
    {
        vkDestroyPipelineLayout(m_Device.device(), m_PipelineLayout, nullptr);
    }

    void VulkanRenderer::OnUpdate()
    {
    }

    void VulkanRenderer::CreatePipelineLayout()
    {
        VkPipelineLayoutCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        info.setLayoutCount = 0;
        info.pSetLayouts = nullptr;
        info.pushConstantRangeCount = 0;
        info.pPushConstantRanges = nullptr;
        
        if (vkCreatePipelineLayout(m_Device.device(), &info, nullptr, &m_PipelineLayout) != VK_SUCCESS)
            LD_CORE_ERROR("Failed to create pipeline layout!");
    }

    void VulkanRenderer::CreatePipeline()
    {
        auto config = VulkanPipeline::DefaultPipelineConfigInfo(m_SwapChain.width(), m_SwapChain.height());
        config.RenderPass = m_SwapChain.getRenderPass();
        config.PipelineLayout = m_PipelineLayout;
        m_Pipeline = CreateScope<VulkanPipeline>(m_Device, config);
    }

    void VulkanRenderer::CreateCommandBuffers()
    {
    }
    
    void VulkanRenderer::DrawFrame()
    {
    }
}