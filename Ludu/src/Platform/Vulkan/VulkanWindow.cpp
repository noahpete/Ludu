#include "ldpch.h"
#include "VulkanWindow.h"

#include "Events/EventManager.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <imgui/imgui_impl_glfw.h>

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

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
            {
                ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
            });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset)
            {
                ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
            });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
            {
                ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
            });

        glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int c)
            {
                ImGui_ImplGlfw_CharCallback(window, c);
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