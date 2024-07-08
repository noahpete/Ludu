#pragma once

#include "Renderer/Camera.h"

#include <vulkan/vulkan.h>

namespace Ludu {
	struct VulkanFrameInfo
	{
		int FrameIndex;
		float FrameTime;
		VkCommandBuffer CommandBuffer;
	};
}