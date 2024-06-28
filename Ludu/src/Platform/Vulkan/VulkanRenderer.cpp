#include "Platform/Vulkan/VulkanRenderer.h"

namespace Ludu
{
    VulkanRenderer::VulkanRenderer(VulkanWindow &window, VulkanDevice &device)
        : m_Window{window}, m_Device{device}
    {
        RecreateSwapChain();
        CreateCommandBuffers();
    }

    VulkanRenderer::~VulkanRenderer()
    {
        FreeCommandBuffers();
    }

    VkCommandBuffer VulkanRenderer::BeginFrame()
    {
        LD_CORE_ASSERT(!m_FrameStarted, "Cannot call BeginFrame while already in progress!");

        auto result = m_SwapChain->acquireNextImage(&m_CurrentImageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            RecreateSwapChain();
            return nullptr;
        }

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
            LD_CORE_ERROR("Failed to acquire swap chain image!");

        m_FrameStarted = true;

        auto commandBuffer = GetCurrentCommandBuffer();
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
            LD_CORE_ERROR("Failed to begin recording command buffer!");

        return commandBuffer;
    }

    void VulkanRenderer::EndFrame()
    {
        LD_CORE_ASSERT(m_FrameStarted, "Cannot call EndFrame while frame is not in progress!");

        auto commandBuffer = GetCurrentCommandBuffer();
        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to record command buffer!");
        }

        auto result = m_SwapChain->submitCommandBuffers(&commandBuffer, &m_CurrentImageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
            m_Window.wasWindowResized())
        {
            m_Window.resetWindowResizedFlag();
            RecreateSwapChain();
        }
        else if (result != VK_SUCCESS)
        {
            throw std::runtime_error("failed to present swap chain image!");
        }

        m_FrameStarted = false;
        m_CurrentFrameIndex = (m_CurrentFrameIndex + 1) % VulkanSwapChain::MAX_FRAMES_IN_FLIGHT;
    }

    void VulkanRenderer::BeginSwapChainRenderPass(VkCommandBuffer commandBuffer)
    {
        LD_CORE_ASSERT(m_FrameStarted, "Cannot call BeginSwapChainRenderPass if frame is not in progress!");
        LD_CORE_ASSERT(commandBuffer == GetCurrentCommandBuffer(), "Cannot begin render pass on command buffer from a different frame!");

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = m_SwapChain->getRenderPass();
        renderPassInfo.framebuffer = m_SwapChain->getFrameBuffer(m_CurrentImageIndex);

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = m_SwapChain->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
        clearValues[1].depthStencil = {1, 0};
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(m_SwapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(m_SwapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{{0, 0}, m_SwapChain->getSwapChainExtent()};
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
    }

    void VulkanRenderer::EndSwapChainRenderPass(VkCommandBuffer commandBuffer)
    {
        LD_CORE_ASSERT(m_FrameStarted, "Cannot call EndSwapChainRenderPass if frame is not in progress!");
        LD_CORE_ASSERT(commandBuffer == GetCurrentCommandBuffer(), "Cannot end render pass on command buffer from a different frame!");

        vkCmdEndRenderPass(commandBuffer);
    }

    void VulkanRenderer::RecreateSwapChain()
    {
        auto extent = m_Window.GetExtent();
        while (extent.width == 0 || extent.height == 0)
        {
            extent = m_Window.GetExtent();
            glfwWaitEvents();
        }
        vkDeviceWaitIdle(m_Device.device());

        if (m_SwapChain == nullptr)
        {
            m_SwapChain = std::make_unique<VulkanSwapChain>(m_Device, extent);
        }
        else
        {
            std::shared_ptr<VulkanSwapChain> oldSwapChain = std::move(m_SwapChain);
            m_SwapChain = std::make_unique<VulkanSwapChain>(m_Device, extent, oldSwapChain);

            if (!oldSwapChain->CompareSwapFormats(*m_SwapChain.get()))
            {
                LD_CORE_ERROR("Swap chain image (or depth) format has changed!");
            }
        }
    }

    void VulkanRenderer::CreateCommandBuffers()
    {
        m_CommandBuffers.resize(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT);

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = m_Device.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(m_CommandBuffers.size());

        if (vkAllocateCommandBuffers(m_Device.device(), &allocInfo, m_CommandBuffers.data()) != VK_SUCCESS)
            LD_CORE_ERROR("Failed to allocate command buffers!");
    }

    void VulkanRenderer::FreeCommandBuffers()
    {
        vkFreeCommandBuffers(m_Device.device(), m_Device.getCommandPool(), static_cast<uint32_t>(m_CommandBuffers.size()), m_CommandBuffers.data());
        m_CommandBuffers.clear();
    }
}