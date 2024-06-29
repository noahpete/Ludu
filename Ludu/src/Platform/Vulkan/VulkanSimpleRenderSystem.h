#pragma once

#include "Core/Core.h"

#include "Platform/Vulkan/VulkanDevice.h"
#include "Platform/Vulkan/VulkanPipeline.h"
#include "Platform/Vulkan/VulkanGameObject.h"
#include "Platform/Vulkan/VulkanCamera.h"
#include "Platform/Vulkan/VulkanPipeline.h"
#include "Platform/Vulkan/VulkanFrameInfo.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace Ludu {

    struct SimplePushConstantData
	{
        glm::mat4 Transform{ 1.0f };
        glm::mat4 ModelMatrix{1.0f};
	};

	class VulkanSimpleRenderSystem
    {
    public:
        VulkanSimpleRenderSystem(VulkanDevice &device, VkRenderPass renderPass);
        ~VulkanSimpleRenderSystem();

        VulkanSimpleRenderSystem(const VulkanSimpleRenderSystem &) = delete;
        VulkanSimpleRenderSystem &operator=(const VulkanSimpleRenderSystem &) = delete;

        void RenderGameObjects(FrameInfo &frameInfo, std::vector<VulkanGameObject> &gameObjects);

    private:
        void CreatePipelineLayout();
        void CreatePipeline(VkRenderPass renderPass);

        VulkanDevice& m_Device;

        std::unique_ptr<VulkanPipeline> m_Pipeline;
        VkPipelineLayout m_PipelineLayout;
        // std::vector<VulkanGameObject> m_GameObjects;

    };
}