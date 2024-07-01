#include "ldpch.h"
#include "VulkanWindow.h"

namespace Ludu
{
    static bool glfwInitialized = false;

    VulkanWindow::VulkanWindow(uint32_t width, uint32_t height, const std::string &title)
    {
        if (!glfwInitialized)
        {
            int result = glfwInit();
            LD_CORE_ASSERT(result == GLFW_TRUE, "Failed to initialize GLFW!");
            glfwInitialized = true;
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        m_Window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    }

    VulkanWindow::~VulkanWindow()
    {
        glfwDestroyWindow(m_Window);
    }

    void VulkanWindow::OnUpdate()
    {
        glfwPollEvents();
    }

    void VulkanWindow::CreateWindowSurface(VkInstance instance, VkSurfaceKHR *surface)
    {
        int success = glfwCreateWindowSurface(instance, m_Window, nullptr, surface);
        if (success != VK_SUCCESS)
            LD_CORE_ERROR("Failed to create window surface!");
    }
}