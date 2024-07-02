#include "ldpch.h"
#include "VulkanContext.h"

#include <GLFW/glfw3.h>

namespace Ludu {

	VulkanContext::VulkanContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{

	}

	void VulkanContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
	}

	void VulkanContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}