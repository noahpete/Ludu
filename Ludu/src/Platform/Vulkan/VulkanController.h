#pragma once

#include "Platform/Vulkan/VulkanGameObject.h"
#include "Platform/Vulkan/VulkanWindow.h"

namespace Ludu {

	class VulkanController
	{
	public:
		struct KeyMappings
		{
			int left = GLFW_KEY_A;
			int right = GLFW_KEY_D;
			int forward = GLFW_KEY_W;
			int backward = GLFW_KEY_S;
			int up = GLFW_KEY_E;
			int down = GLFW_KEY_Q;
			int lookLeft = GLFW_KEY_LEFT;
			int lookRight = GLFW_KEY_RIGHT;
			int lookUp = GLFW_KEY_UP;
			int lookDown = GLFW_KEY_DOWN;
		};

		KeyMappings m_Keys{};
		float m_MoveSpeed = 3.0f;
		float m_LookSpeed = 1.5f;

		void MoveInPlaneXZ(GLFWwindow* window, float dt, VulkanGameObject& gameObject);

	};
}