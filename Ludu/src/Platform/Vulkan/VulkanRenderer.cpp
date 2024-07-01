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
        DrawFrame();
    }

    void VulkanRenderer::Shutdown()
    {
        vkDeviceWaitIdle(m_Device.device());
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
        m_CommandBuffers.resize(m_SwapChain.imageCount());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = m_Device.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(m_CommandBuffers.size());

        if (vkAllocateCommandBuffers(m_Device.device(), &allocInfo, m_CommandBuffers.data()) != VK_SUCCESS)
            LD_CORE_ERROR("Failed to allocate command buffers!");

        for (int i = 0; i < m_CommandBuffers.size(); i++)
        {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if (vkBeginCommandBuffer(m_CommandBuffers[i], &beginInfo) != VK_SUCCESS)
                LD_CORE_ERROR("Failed to begin recording command buffer!");

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = m_SwapChain.getRenderPass();
            renderPassInfo.framebuffer = m_SwapChain.getFrameBuffer(i);

            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = m_SwapChain.getSwapChainExtent();

            std::array<VkClearValue, 2> clearValues{};
            clearValues[0].color = {0.1, 0.1, 0.1, 1.0f};
            clearValues[1].depthStencil = {1, 0};
            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(m_CommandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            m_Pipeline->Bind(m_CommandBuffers[i]);
            vkCmdDraw(m_CommandBuffers[i], 3, 1, 0, 0);

            vkCmdEndRenderPass(m_CommandBuffers[i]);
            if (vkEndCommandBuffer(m_CommandBuffers[i]) != VK_SUCCESS)
                LD_CORE_ERROR("Failed to record command buffer!");
        }
    }

    void VulkanRenderer::DrawFrame()
    {
        uint32_t imageIndex;
        auto result = m_SwapChain.acquireNextImage(&imageIndex);

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
            LD_CORE_ERROR("Failed to acquire swap chain image!");

        result = m_SwapChain.submitCommandBuffers(&m_CommandBuffers[imageIndex], &imageIndex);

        if (result != VK_SUCCESS)
            LD_CORE_ERROR("Failed to present swap chain image!");
    }
}