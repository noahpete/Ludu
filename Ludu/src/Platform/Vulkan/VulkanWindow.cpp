#include "VulkanWindow.h"
#include "VulkanWindow.h"

#include "Core/Core.h"

namespace Ludu {

    VulkanWindow::VulkanWindow(uint32_t width, uint32_t height, const std::string& title)
    : width(width), height(height), title(title)
    {
        initWindow();
    }

    VulkanWindow::~VulkanWindow()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void VulkanWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface)
    {
        if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
            LD_CORE_ERROR("Failed to create window surface!");
    }

    void VulkanWindow::initWindow()
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    }

    void Ludu::VulkanWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height)
    {
        auto vulkanWindow = reinterpret_cast<VulkanWindow*>(glfwGetWindowUserPointer(window));
        vulkanWindow->frameBufferResized = true;
        vulkanWindow->width = width;
        vulkanWindow->height = height;
    }
}