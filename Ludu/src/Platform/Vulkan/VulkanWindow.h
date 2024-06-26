#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace Ludu
{

    class VulkanWindow
    {
    public:
        VulkanWindow(uint32_t width, uint32_t height, const std::string& title);
        ~VulkanWindow();

        VulkanWindow(const VulkanWindow&) = delete;
        VulkanWindow &operator=(const VulkanWindow&) = delete;

        VkExtent2D GetExtent() { return { width, height }; }

        bool shouldClose() { return glfwWindowShouldClose(window); }

        void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

    private:
        const uint32_t width, height;
        std::string title;
        GLFWwindow *window;

        void initWindow();
    };
}