#pragma once

#include "Platform/Vulkan/VulkanCamera.h"

#include <vulkan/vulkan.h>

namespace Ludu {

	struct FrameInfo
	{
		int frameIndex;
		float frameTime;
		VkCommandBuffer commandBuffer;
		VulkanCamera& camera;
		VkDescriptorSet globalDescriptorSet;
	};
}