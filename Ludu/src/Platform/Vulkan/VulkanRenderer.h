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

        void Submit(Entity* entity) override;

        void BeginFrame();
        void EndFrame();

        VulkanRenderer(const VulkanRenderer &) = delete;
        VulkanRenderer &operator=(const VulkanRenderer &) = delete;

    private:
        VulkanDevice m_Device;
        Ref<VulkanWindow> m_Window;
        Scope<VulkanSwapChain> m_SwapChain;
        Scope<VulkanPipeline> m_Pipeline;
        VkPipelineLayout m_PipelineLayout;
        std::vector<VkCommandBuffer> m_CommandBuffers;

        std::vector<Entity*> m_RenderQueue;

        uint32_t m_ImageIndex;
        bool m_FrameStarted;

        void CreatePipelineLayout();
        void CreatePipeline();
        void CreateCommandBuffers();
        void FreeCommandBuffers();

        void RecreateSwapChain();
        void RecordCommandBuffer(int imageIndex);
        void RenderScene(VkCommandBuffer commandBuffer);
        
    };
}