#include "ldpch.h"
#include "VulkanWindow.h"

#include "Events/EventManager.h"

namespace Ludu
{
    static bool glfwInitialized = false;

    VulkanWindow::VulkanWindow(uint32_t width, uint32_t height, const std::string &title)
        : m_Width(width), m_Height(height), m_Title(title)
    {
        if (!glfwInitialized)
        {
            int result = glfwInit();
            LD_CORE_ASSERT(result == GLFW_TRUE, "Failed to initialize GLFW!");
            glfwInitialized = true;
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        m_Window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

        glfwSetWindowUserPointer(m_Window, this);
        glfwSetFramebufferSizeCallback(m_Window, OnFrameBufferResize);

        // Set GLFW callbacks
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
        {
            EventManager::Publish<WindowResizeEvent>(WindowResizeEvent(width, height));
        });
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
        int result = glfwCreateWindowSurface(instance, m_Window, nullptr, surface);
        if (result != VK_SUCCESS)
            LD_CORE_ERROR("Failed to create window surface!");
    }

    void VulkanWindow::OnFrameBufferResize(GLFWwindow* window, int width, int height)
    {
        auto vulkanWindow = reinterpret_cast<VulkanWindow*>(glfwGetWindowUserPointer(window));
        vulkanWindow->m_FrameBufferResized = true;
        vulkanWindow->m_Width = width;
        vulkanWindow->m_Height = height;
    }
}