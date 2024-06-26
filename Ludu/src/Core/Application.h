#pragma once

#include "Core/Window.h"
#include "Events/EventManager.h"

// Temporary
#include "Platform/Vulkan/VulkanDevice.h"
#include "Platform/Vulkan/VulkanPipeline.h"
#include "Platform/Vulkan/VulkanSwapChain.h"
#include "Platform/Vulkan/VulkanWindow.h"

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
		Scope<Window> m_Window;
		bool m_Running;

	};

	// To be defined in client
	Application* CreateApplication();
}