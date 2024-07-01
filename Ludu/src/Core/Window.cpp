#include "ldpch.h"
#include "Window.h"

#include "Platform/Vulkan/VulkanWindow.h"

namespace Ludu
{
	Scope<Window> Window::Create(uint32_t width, uint32_t height, const std::string& title)
    {
        return CreateScope<VulkanWindow>(width, height, title);
    }
}