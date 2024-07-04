#include "VulkanRenderer.h"
#include "VulkanRenderer.h"
#include "ldpch.h"
#include "VulkanRenderer.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include "Scene/Components.h"

namespace Ludu
{
    Renderer* Renderer::s_Instance = nullptr;

    struct SimplePushConstantData
    {
        glm::mat4 transform{1.0f};
        alignas(16) glm::vec3 color;
    };

    VulkanRenderer::VulkanRenderer(Ref<VulkanWindow> window)
        : m_Window(window), m_Device{ *window }, m_Pipeline{}
    {
        s_Instance = this;

        CreatePipelineLayout();
        RecreateSwapChain();
        CreateCommandBuffers();
    }

    VulkanRenderer::~VulkanRenderer()
    {
        vkDestroyPipelineLayout(m_Device.device(), m_PipelineLayout, nullptr);
    }

    void VulkanRenderer::OnUpdate()
    {
        BeginFrame();
        RenderScene(m_CommandBuffers[m_ImageIndex]);
        EndFrame();
    }

    void VulkanRenderer::Shutdown()
    {
        vkDeviceWaitIdle(m_Device.device());
    }

    void VulkanRenderer::Submit(Entity* entity)
    {
        m_RenderQueue.push_back(entity);
    }

    void VulkanRenderer::BeginFrame()
    {
        LD_CORE_ASSERT(!m_FrameStarted);

        auto result = m_SwapChain->acquireNextImage(&m_ImageIndex);

        // Resize window
        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            RecreateSwapChain();
            return;
        }

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
            LD_CORE_ERROR("Failed to acquire swap chain image!");

        m_FrameStarted = true;

        // Begin swap chain render pass
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(m_CommandBuffers[m_ImageIndex], &beginInfo) != VK_SUCCESS)
            LD_CORE_ERROR("Failed to begin recording command buffer!");

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = m_SwapChain->getRenderPass();
        renderPassInfo.framebuffer = m_SwapChain->getFrameBuffer(m_ImageIndex);

        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = m_SwapChain->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = { 0.1, 0.1, 0.1, 1.0f };
        clearValues[1].depthStencil = { 1, 0 };
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(m_CommandBuffers[m_ImageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(m_SwapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(m_SwapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{ {0, 0}, m_SwapChain->getSwapChainExtent() };
        vkCmdSetViewport(m_CommandBuffers[m_ImageIndex], 0, 1, &viewport);
        vkCmdSetScissor(m_CommandBuffers[m_ImageIndex], 0, 1, &scissor);
    }

    void VulkanRenderer::EndFrame()
    {
        // End swap chain render pass
        vkCmdEndRenderPass(m_CommandBuffers[m_ImageIndex]);
        if (vkEndCommandBuffer(m_CommandBuffers[m_ImageIndex]) != VK_SUCCESS)
            LD_CORE_ERROR("Failed to record command buffer!");

        auto result = m_SwapChain->submitCommandBuffers(&m_CommandBuffers[m_ImageIndex], &m_ImageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_Window->WasWindowResized())
        {
            m_Window->ResetWindowResizedFlag();
            RecreateSwapChain();
        }

        //if (result != VK_SUCCESS)
        //    LD_CORE_ERROR("Failed to present swap chain image!");

        m_FrameStarted = false;
    }

    void VulkanRenderer::CreatePipelineLayout()
    {
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(SimplePushConstantData);

        VkPipelineLayoutCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        info.setLayoutCount = 0;
        info.pSetLayouts = nullptr;
        info.pushConstantRangeCount = 1;
        info.pPushConstantRanges = &pushConstantRange;

        if (vkCreatePipelineLayout(m_Device.device(), &info, nullptr, &m_PipelineLayout) != VK_SUCCESS)
            LD_CORE_ERROR("Failed to create pipeline layout!");
    }

    void VulkanRenderer::CreatePipeline()
    {
        PipelineConfigInfo pipelineConfig{};
        VulkanPipeline::DefaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.RenderPass = m_SwapChain->getRenderPass();
        pipelineConfig.PipelineLayout = m_PipelineLayout;
        m_Pipeline = CreateScope<VulkanPipeline>(m_Device, pipelineConfig);
    }

    void VulkanRenderer::CreateCommandBuffers()
    {
        m_CommandBuffers.resize(m_SwapChain->imageCount());

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
        vkFreeCommandBuffers(m_Device.device(), m_Device.getCommandPool(), static_cast<float>(m_CommandBuffers.size()), m_CommandBuffers.data());
        m_CommandBuffers.clear();
    }

    void VulkanRenderer::RecreateSwapChain()
    {
        auto extent = m_Window->GetExtent();
        while (extent.width == 0 || extent.height == 0)
        {
            extent = m_Window->GetExtent();
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(m_Device.device());

        if (!m_SwapChain)
            m_SwapChain = CreateScope<VulkanSwapChain>(m_Device, extent);
        else
        {
            m_SwapChain = CreateScope<VulkanSwapChain>(m_Device, extent, std::move(m_SwapChain));
            if (m_SwapChain->imageCount() != m_CommandBuffers.size())
            {
                FreeCommandBuffers();
                CreateCommandBuffers();
            }
        }

        CreatePipeline();
    }

    void VulkanRenderer::RenderScene(VkCommandBuffer commandBuffer)
    {
        m_Pipeline->Bind(commandBuffer);

        for (auto entity : m_RenderQueue)
        {
            SimplePushConstantData push{};
            push.color = entity->GetComponent<MeshComponent>().Color;
            push.transform = entity->GetComponent<TransformComponent>().GetTransform();

            vkCmdPushConstants(commandBuffer, m_PipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);

            entity->GetComponent<MeshComponent>().Model->Bind(commandBuffer);
            entity->GetComponent<MeshComponent>().Model->Draw(commandBuffer);
        }

        m_RenderQueue.clear();
    }
}