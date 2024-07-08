#include "VulkanRenderer.h"
#include "ldpch.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vulkan/vulkan.h>

#include "Scene/Components.h"

namespace Ludu
{
    Renderer* Renderer::s_Instance = nullptr;

    struct SimplePushConstantData
    {
        glm::mat4 transform{ 1.0f };
        glm::mat4 normalMatrix{1.0f};
       
    };

    struct GlobalUbo
    {
        glm::mat4 ProjectionView{ 1.0f };
        glm::vec3 LightDirection = glm::normalize(glm::vec3{ 1.0f, -3.0f, -1.0f });
    };

    VulkanRenderer::VulkanRenderer(Ref<VulkanWindow> window)
        : m_Window(window), m_Device{ *window }, m_Pipeline{}
    {
        s_Instance = this;

        for (int i = 0; i < VulkanSwapChain::MAX_FRAMES_IN_FLIGHT; i++)
        {
            m_UboBuffers.emplace_back(CreateScope<VulkanBuffer>(m_Device, sizeof(GlobalUbo), 1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT));
            m_UboBuffers.back()->map();
        }

        CreatePipelineLayout();
        RecreateSwapChain();
        CreateCommandBuffers();

        m_GlobalPool = VulkanDescriptorPool::Builder(m_Device)
            .setMaxSets(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT)
            .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VulkanSwapChain::MAX_FRAMES_IN_FLIGHT)
            .build();

        auto globalSetLayout = VulkanDescriptorLayout::Builder(m_Device)
            .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
            .build();

        m_GlobalDescriptorSets.resize(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < VulkanSwapChain::MAX_FRAMES_IN_FLIGHT; i++)
        {
            auto bufferInfo = m_UboBuffers[i]->descriptorInfo();
            VulkanDescriptorWriter(*globalSetLayout, *m_GlobalPool)
                .writeBuffer(0, &bufferInfo)
                .build(m_GlobalDescriptorSets[i]);
        }
    }

    VulkanRenderer::~VulkanRenderer()
    {
        vkDestroyPipelineLayout(m_Device.device(), m_PipelineLayout, nullptr);
    }

    void VulkanRenderer::OnUpdate(const Camera& camera)
    {
        RenderScene(camera);
    }

    void VulkanRenderer::Shutdown()
    {
        vkDeviceWaitIdle(m_Device.device());

        // ImGui shutdown
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        vkDestroyDescriptorPool(m_Device.device(), m_ImGuiPool, nullptr);
    }

    void VulkanRenderer::Submit(Entity* entity)
    {
        m_RenderQueue.push_back(entity);
    }

    void VulkanRenderer::Begin(const Camera& camera)
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

        // Update
        GlobalUbo ubo{};
        ubo.ProjectionView = camera.GetProjection() * camera.GetView();
        m_UboBuffers[m_FrameIndex]->writeToBuffer(&ubo);
        m_UboBuffers[m_FrameIndex]->flush();

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

    void VulkanRenderer::End()
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
        m_FrameIndex = (m_FrameIndex + 1) % VulkanSwapChain::MAX_FRAMES_IN_FLIGHT;
    }

    void VulkanRenderer::InitImGui()
    {
        VkDescriptorPoolSize pool_sizes[] =
        {
            { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
        };

        VkDescriptorPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets = 1000;
        pool_info.poolSizeCount = std::size(pool_sizes);
        pool_info.pPoolSizes = pool_sizes;

        vkCreateDescriptorPool(m_Device.device(), &pool_info, nullptr, &m_ImGuiPool);


        // 2: initialize imgui library

        //this initializes the core structures of imgui
        ImGui::CreateContext();

        //this initializes imgui for SDL
        ImGui_ImplGlfw_InitForVulkan(reinterpret_cast<GLFWwindow*>(m_Window->GetNativeWindow()), false);

        //this initializes imgui for Vulkan
        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = m_Device.GetInstance();
        init_info.PhysicalDevice = m_Device.getPhysicalDevice();
        init_info.Device = m_Device.device();
        init_info.Queue = m_Device.graphicsQueue();
        init_info.DescriptorPool = m_ImGuiPool;
        init_info.MinImageCount = 3;
        init_info.ImageCount = 3;
        init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        init_info.RenderPass = m_SwapChain->getRenderPass();

        ImGui_ImplVulkan_Init(&init_info);
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


    void VulkanRenderer::RenderScene(const Camera& camera)
    {
        m_Pipeline->Bind(m_CommandBuffers[m_ImageIndex]);

        auto projectionView = camera.GetProjection() * camera.GetView();

        for (auto entity : m_RenderQueue)
        {
            SimplePushConstantData push{};
            auto modelMatrix = entity->GetComponent<TransformComponent>().GetTransform();
            push.transform = projectionView * modelMatrix;
            push.normalMatrix = entity->GetComponent<TransformComponent>().GetNormalMatrix();

            vkCmdPushConstants(m_CommandBuffers[m_ImageIndex], m_PipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);

            entity->GetComponent<MeshComponent>().Model->Bind(m_CommandBuffers[m_ImageIndex]);
            entity->GetComponent<MeshComponent>().Model->Draw(m_CommandBuffers[m_ImageIndex]);
        }

        m_RenderQueue.clear();
    }
}