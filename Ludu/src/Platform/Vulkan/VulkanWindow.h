#pragma once

#include "Core/Window.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Ludu
{

    class VulkanWindow : public Window
    {
    public:
        VulkanWindow(uint32_t width, uint32_t height, const std::string& title);
        ~VulkanWindow();

        void OnUpdate() override;

        uint32_t GetWidth() const override { return m_Width; }
        uint32_t GetHeight() const override { return m_Height; }

        VkExtent2D GetExtent() const { return { m_Width, m_Height }; }

        VulkanWindow(const VulkanWindow &) = delete;
        VulkanWindow &operator=(const VulkanWindow &) = delete;

        bool ShouldClose() override { return glfwWindowShouldClose(m_Window); }
        bool WasWindowResized() { return m_FrameBufferResized; }
        void ResetWindowResizedFlag() { m_FrameBufferResized = false; }

        void CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

    private:
        uint32_t m_Width, m_Height;
        std::string m_Title;
        bool m_FrameBufferResized = false;

        GLFWwindow *m_Window;

        static void OnFrameBufferResize(GLFWwindow* window, int width, int height);

    };
}