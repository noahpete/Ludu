#pragma once

#include "Core/Window.h"
#include "Events/EventManager.h"

#include "Platform/Vulkan/VulkanWindow.h"
#include "Platform/Vulkan/VulkanDevice.h"
#include "Platform/Vulkan/VulkanPipeline.h"
#include "Platform/Vulkan/VulkanSwapChain.h"
#include "Platform/Vulkan/VulkanModel.h"


namespace Ludu {

	class VulkanRenderer
	{
	public:
		VulkanRenderer();
		virtual ~VulkanRenderer();

		void Run();

		void OnWindowQuitEvent(QuitEvent& event);
		void OnWindowResizeEvent(WindowResizeEvent& event);

	private:
		static VulkanRenderer* s_Instance;
		bool m_Running;

		// Temporary
		VulkanWindow m_Window{1280, 720, "Hello Vulkan!"};
		VulkanDevice m_Device{m_Window};
		std::unique_ptr<VulkanSwapChain> m_SwapChain;
		std::unique_ptr<VulkanPipeline> m_Pipeline;
		VkPipelineLayout m_PipelineLayout;
		std::vector<VkCommandBuffer> m_CommandBuffers;
		std::unique_ptr<VulkanModel> m_Model;

		void CreatePipelineLayout();
		void CreatePipeline();
		void CreateCommandBuffers();
		void FreeCommandBuffers();
		void DrawFrame();
		void LoadModels();
		void RecreateSwapChain();
		void RecordCommandBuffer(int imageIndex);

	};
}