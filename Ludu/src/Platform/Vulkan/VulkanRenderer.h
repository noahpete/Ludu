#pragma once

#include "Renderer/Renderer.h"
#include "Platform/Vulkan/VulkanWindow.h"
#include "Platform/Vulkan/VulkanPipeline.h"
#include "Platform/Vulkan/VulkanDevice.h"

namespace Ludu
{

    class VulkanRenderer : public Renderer
    {
    public:
        VulkanRenderer(Ref<VulkanWindow> window);
        ~VulkanRenderer();

        void OnUpdate() override;

    private:
        VulkanDevice m_Device;
        VulkanPipeline m_Pipeline;
        
    };
}