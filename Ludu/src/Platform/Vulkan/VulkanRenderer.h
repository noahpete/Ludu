#pragma once

#include "Renderer/Renderer.h"
#include "Platform/Vulkan/VulkanWindow.h"

namespace Ludu
{

    class VulkanRenderer : public Renderer
    {
    public:
        VulkanRenderer(Ref<VulkanWindow> window);
        ~VulkanRenderer();

        void OnUpdate() override;

    };
}