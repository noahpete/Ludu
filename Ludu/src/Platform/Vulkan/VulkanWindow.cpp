#include "ldpch.h"
#include "VulkanWindow.h"
#include "Platform/Vulkan/VulkanWindow.h"

#include "Core/Window.h"
#include "Events/EventManager.h"

namespace Ludu {

	static bool s_GLFWInitialized = false;

	Window* Window::Create(const std::string& title, uint32_t width, uint32_t height)
	{
		return new VulkanWindow(title, width, height);
	}

	VulkanWindow::VulkanWindow(const std::string& title, uint32_t width, uint32_t height)
	{
		Init(title, width, height);
	}

	VulkanWindow::~VulkanWindow()
	{
		Shutdown();
	}

	void VulkanWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void VulkanWindow::CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
	{
		if (instance == VK_NULL_HANDLE) {
			LD_CORE_ERROR("Invalid Vulkan instance passed to CreateWindowSurface");
			return;
		}

		if (surface == nullptr) {
			LD_CORE_ERROR("Surface is null in CreateWindowSurface");
			return;
		}

		VkResult result = glfwCreateWindowSurface(instance, m_Window, nullptr, surface);
		if (result != VK_SUCCESS) {
			LD_CORE_ERROR("Failed to create window surface! Error code: {0}", static_cast<int>(result));
		}
	}

	void VulkanWindow::Init(const std::string& title, uint32_t width, uint32_t height)
	{
		m_Title = title;
		m_Width = width;
		m_Height = height;

		LD_CORE_INFO("Creating window {0} ({1}, {2})", title, width, height);

		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			LD_CORE_ASSERT(success, "Could not initialize GLFW!");
			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		//glfwSetWindowUserPointer(m_Window, m_Data);
		glfwSwapInterval(1);

		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				EventManager::Publish<WindowResizeEvent>(WindowResizeEvent(width, height));
			});


	}

	void VulkanWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

}