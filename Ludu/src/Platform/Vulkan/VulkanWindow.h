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
        bool wasWindowResized() { return frameBufferResized; }
        void resetWindowResizedFlag() { frameBufferResized = false; }

        void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

        GLFWwindow* GetGLFWWindow() const { return window; }

    private:
        uint32_t width, height;
        std::string title;
        GLFWwindow *window;
        bool frameBufferResized = false;

        void initWindow();
        static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
    };
}