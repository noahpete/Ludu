#include "Application.h"
#include "Application.h"
#include "Application.h"

namespace Ludu {

	Application* Application::s_Instance = nullptr;

	Application::Application()
		: m_Running(true)
	{
		LoadModels();
		CreatePipelineLayout();
		RecreateSwapChain();
		CreateCommandBuffers();
	}

	Application::~Application()
	{
		vkDestroyPipelineLayout(m_Device.device(), m_PipelineLayout, nullptr);
	}

	void Application::Run()
	{
		while (!m_Window.shouldClose())
		{
			glfwPollEvents();
			DrawFrame();
		}

		vkDeviceWaitIdle(m_Device.device());
	}

    void Application::CreatePipelineLayout()
    {
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;

		if (vkCreatePipelineLayout(m_Device.device(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS)
			LD_CORE_ERROR("Failed to create pipeline layout!");
    }

    void Application::CreatePipeline()
    {
		PipelineConfigInfo pipelineConfig{};
		VulkanPipeline::defaultPipelineConfigInfo(pipelineConfig);
		pipelineConfig.RenderPass = m_SwapChain->getRenderPass();
		pipelineConfig.PipelineLayout = m_PipelineLayout;
		m_Pipeline = std::make_unique<VulkanPipeline>(m_Device, "simple_shader.vert.spv", "simple_shader.frag.spv", pipelineConfig);
    }

    void Application::CreateCommandBuffers()
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

	void Application::FreeCommandBuffers()
	{
		vkFreeCommandBuffers(m_Device.device(), m_Device.getCommandPool(), m_CommandBuffers.size(), m_CommandBuffers.data());
		m_CommandBuffers.clear();
	}

    void Application::DrawFrame()
    {
		uint32_t imageIndex;
		auto result = m_SwapChain->acquireNextImage(&imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			RecreateSwapChain();
			return;
		}

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			LD_CORE_ERROR("Failed to acquire swap chain image!");

		RecordCommandBuffer(imageIndex);
		result = m_SwapChain->submitCommandBuffers(&m_CommandBuffers[imageIndex], &imageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_Window.wasWindowResized())
		{
			m_Window.resetWindowResizedFlag();
			RecreateSwapChain();
			return;
		}

		if (result != VK_SUCCESS)
			LD_CORE_ERROR("Failed to present swap chain image!");
    }

	void Application::LoadModels()
	{
		std::vector<VulkanModel::Vertex> vertices{
			{ { 0.0f, -0.5f }, { 1.0f, 0.0f, 0.0f} },
			{ { 0.5f,  0.5f }, { 0.0f, 1.0f, 0.0f} },
			{ {-0.5f,  0.5f }, { 0.0f, 0.0f, 1.0f} }
		};

		m_Model = std::make_unique<VulkanModel>(m_Device, vertices);
	}
	
	void Application::RecreateSwapChain()
	{
		vkDeviceWaitIdle(m_Device.device()); // Ensure the device is idle before starting the recreation process

		// Cleanup existing swap chain and related resources
		m_CommandBuffers.clear(); // Clear the command buffers
		m_Pipeline.reset();       // Reset the pipeline
		m_SwapChain.reset();      // Reset the swap chain

		auto extent = m_Window.GetExtent();
		while (extent.width == 0 || extent.height == 0) {
			extent = m_Window.GetExtent();
			glfwWaitEvents();
		}

		if (m_SwapChain == nullptr)
		{
			m_SwapChain = std::make_unique<VulkanSwapChain>(m_Device, extent);
		}
		else
		{
			m_SwapChain = std::make_unique<VulkanSwapChain>(m_Device, extent, std::move(m_SwapChain));
			if (m_SwapChain->imageCount() != m_CommandBuffers.size())
			{
				FreeCommandBuffers();
				CreateCommandBuffers();
			}
		}

		// Recreate the pipeline with the new swap chain render pass and extent
		CreatePipeline();

		// Recreate the command buffers with the new swap chain framebuffers
		CreateCommandBuffers();
	}


	void Application::RecordCommandBuffer(int imageIndex)
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(m_CommandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
			LD_CORE_ERROR("Failed to begin recording command buffer!");

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_SwapChain->getRenderPass();
		renderPassInfo.framebuffer = m_SwapChain->getFrameBuffer(imageIndex);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = m_SwapChain->getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
		clearValues[1].depthStencil = { 1, 0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(m_CommandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(m_SwapChain->getSwapChainExtent().width);
		viewport.height = static_cast<float>(m_SwapChain->getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{ {0, 0}, m_SwapChain->getSwapChainExtent() };
		vkCmdSetViewport(m_CommandBuffers[imageIndex], 0, 1, &viewport);
		vkCmdSetScissor(m_CommandBuffers[imageIndex], 0, 1, &scissor);

		m_Pipeline->Bind(m_CommandBuffers[imageIndex]);
		m_Model->Bind(m_CommandBuffers[imageIndex]);
		m_Model->Draw(m_CommandBuffers[imageIndex]);

		vkCmdEndRenderPass(m_CommandBuffers[imageIndex]);
		if (vkEndCommandBuffer(m_CommandBuffers[imageIndex]) != VK_SUCCESS)
			LD_CORE_ERROR("Failed to record command buffer!");
	}
}
