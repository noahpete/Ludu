#pragma once

#include "Core/Window.h"
#include "Events/EventManager.h"

#include "Platform/Vulkan/VulkanWindow.h"
#include "Platform/Vulkan/VulkanDevice.h"
#include "Platform/Vulkan/VulkanGameObject.h"
#include "Platform/Vulkan/VulkanRenderer.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

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
		std::vector<VulkanGameObject> m_GameObjects;
		VulkanRenderer m_Renderer{m_Window, m_Device};

		void LoadGameObjects();

	};

	// To be defined in client
	Application* CreateApplication();
}