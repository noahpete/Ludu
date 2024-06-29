#pragma once

#include "Core/Core.h"

#include "Platform/Vulkan/VulkanWindow.h"
#include "Platform/Vulkan/VulkanDevice.h"
#include "Platform/Vulkan/VulkanSwapChain.h"
#include "Platform/Vulkan/VulkanModel.h"

namespace Ludu
{

	class VulkanRenderer
	{
	public:
		VulkanRenderer(VulkanWindow &window, VulkanDevice &device);
		virtual ~VulkanRenderer();

		VulkanRenderer(const VulkanRenderer &) = delete;
		VulkanRenderer &operator=(const VulkanRenderer &) = delete;

		VkCommandBuffer BeginFrame();
		void EndFrame();
		void BeginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void EndSwapChainRenderPass(VkCommandBuffer commandBuffer);

		VkRenderPass GetSwapChainRenderPass() const { return m_SwapChain->getRenderPass(); }
		float GetAspectRatio() const { return m_SwapChain->extentAspectRatio(); }
		bool IsFrameInProgress() const { return m_FrameStarted; }
		VkCommandBuffer GetCurrentCommandBuffer() const
		{
			LD_CORE_ASSERT(m_FrameStarted, "Cannot get command buffer when frame not in progress!");
			return m_CommandBuffers[m_CurrentFrameIndex];
		}

		int GetFrameIndex() const { return m_CurrentFrameIndex; }

	private:
		VulkanWindow &m_Window;
		VulkanDevice &m_Device;
		std::unique_ptr<VulkanSwapChain> m_SwapChain;
		std::vector<VkCommandBuffer> m_CommandBuffers;

		uint32_t m_CurrentImageIndex;
		int m_CurrentFrameIndex = 0;
		bool m_FrameStarted = false;

		void CreateCommandBuffers();
		void FreeCommandBuffers();
		void RecreateSwapChain();
	};
}