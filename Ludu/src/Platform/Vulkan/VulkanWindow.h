#pragma once

#include "Core/Window.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Ludu {

	class VulkanWindow : public Window
	{
	public:
		VulkanWindow(const std::string& title, uint32_t width, uint32_t height);
		virtual ~VulkanWindow();

		void OnUpdate() override;

		inline uint32_t GetWidth() const override { return m_Width; }
		inline uint32_t GetHeight() const override { return m_Height; }
		inline VkExtent2D GetExtent() const { return { m_Width, m_Height }; }

		void CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

	private:
		GLFWwindow* m_Window;
		std::string m_Title;
		uint32_t m_Width, m_Height;

		void Init(const std::string& title, uint32_t width, uint32_t height);
		void Shutdown();

	};
}