#pragma once

#include "Renderer/Renderer.h"
#include "Platform/Vulkan/VulkanWindow.h"
#include "Platform/Vulkan/VulkanPipeline.h"
#include "Platform/Vulkan/VulkanDevice.h"
#include "Platform/Vulkan/VulkanSwapChain.h"
#include "Platform/Vulkan/VulkanModel.h"

namespace Ludu
{

    class VulkanRenderer : public Renderer
    {
    public:
        VulkanRenderer(Ref<VulkanWindow> window);
        ~VulkanRenderer();

        void OnUpdate() override;

        void Shutdown() override;

        VulkanRenderer(const VulkanRenderer &) = delete;
        VulkanRenderer &operator=(const VulkanRenderer &) = delete;

    private:
        VulkanDevice m_Device;
        Ref<VulkanWindow> m_Window;
        Scope<VulkanSwapChain> m_SwapChain;
        Scope<VulkanPipeline> m_Pipeline;
        VkPipelineLayout m_PipelineLayout;
        std::vector<VkCommandBuffer> m_CommandBuffers;

        // Temp
        Scope<VulkanModel> m_Model;
        void LoadModels();

        void CreatePipelineLayout();
        void CreatePipeline();
        void CreateCommandBuffers();
        void FreeCommandBuffers();
        void DrawFrame();

        void RecreateSwapChain();
        void RecordCommandBuffer(int imageIndex);
        
    };
}