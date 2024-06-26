#pragma once

#include "Core/Window.h"
#include "Events/EventManager.h"

#include "Platform/Vulkan/VulkanWindow.h"
#include "Platform/Vulkan/VulkanDevice.h"
#include "Platform/Vulkan/VulkanPipeline.h"
#include "Platform/Vulkan/VulkanSwapChain.h"


int main(int argc, char* argv[]);

namespace Ludu {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnWindowQuitEvent(QuitEvent& event);
		void OnWindowResizeEvent(WindowResizeEvent& event);

		static Application& Get() { return *s_Instance; }

	private:
		static Application* s_Instance;
		bool m_Running;

		// Temporary
		VulkanWindow m_Window{1280, 720, "Hello Vulkan!"};
		VulkanDevice m_Device{m_Window};
		VulkanSwapChain m_SwapChain{m_Device, m_Window.GetExtent()};
		std::unique_ptr<VulkanPipeline> m_Pipeline;
		VkPipelineLayout m_PipelineLayout;
		std::vector<VkCommandBuffer> m_CommandBuffers;

		void CreatePipelineLayout();
		void CreatePipeline();
		void CreateCommandBuffers();
		void DrawFrame();

	};

	// To be defined in client
	Application* CreateApplication();
}