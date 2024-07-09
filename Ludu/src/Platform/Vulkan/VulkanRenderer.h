#pragma once

#include "Renderer/Renderer.h"
#include "Renderer/Camera.h"

#include "Platform/Vulkan/VulkanWindow.h"
#include "Platform/Vulkan/VulkanPipeline.h"
#include "Platform/Vulkan/VulkanDevice.h"
#include "Platform/Vulkan/VulkanSwapChain.h"
#include "Platform/Vulkan/VulkanModel.h"
#include "Platform/Vulkan/VulkanBuffer.h"
#include "Platform/Vulkan/VulkanDescriptors.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_vulkan.h>

namespace Ludu
{

    class VulkanRenderer : public Renderer
    {
    public:
        VulkanRenderer(Ref<VulkanWindow> window);
        ~VulkanRenderer();

        void OnUpdate(const Camera& camera) override;
        void Shutdown() override;

        void Submit(Entity* entity) override;

        void Begin(const Camera& camera) override;
        void End() override;

        void InitImGui() override;

        float GetAspectRatio() const override { return m_SwapChain->extentAspectRatio(); }
        VkCommandBuffer GetCurrentCommandBuffer() const { return m_CommandBuffers[m_ImageIndex]; }

        VulkanRenderer(const VulkanRenderer &) = delete;
        VulkanRenderer &operator=(const VulkanRenderer &) = delete;

    private:
        VulkanDevice m_Device;
        Ref<VulkanWindow> m_Window;
        Scope<VulkanSwapChain> m_SwapChain;
        Scope<VulkanPipeline> m_Pipeline;
        VkPipelineLayout m_PipelineLayout;
        std::vector<VkCommandBuffer> m_CommandBuffers;
        VkDescriptorPool m_ImGuiPool;

        std::vector<Entity*> m_RenderQueue;
        uint32_t m_ImageIndex;
        uint32_t m_FrameIndex = 0;
        bool m_FrameStarted;

        std::vector<Scope<VulkanBuffer>> m_UboBuffers{};
        std::vector<VkDescriptorSet> m_GlobalDescriptorSets{};

        Scope<VulkanDescriptorPool> m_GlobalPool{};

        void CreatePipelineLayout(VkDescriptorSetLayout globalSetLayout);
        void CreatePipeline();
        void CreateCommandBuffers();
        void FreeCommandBuffers();

        void RecreateSwapChain();
        void RenderScene(const Camera& camera);
        
    };
}