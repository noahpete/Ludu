#include "ldpch.h"
#include "Renderer.h"

#include "Platform/Vulkan/VulkanRenderer.h"

#include <memory>

namespace Ludu {

    Scope<Renderer> Renderer::Create(Ref<Window> window)
    {
        return CreateScope<VulkanRenderer>(std::static_pointer_cast<VulkanWindow>(window));
    }
}