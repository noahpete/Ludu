#pragma once

#include "Renderer/GraphicsContext.h"

class GLFWwindow;

namespace Ludu {

	class VulkanContext : public GraphicsContext
	{
	public:
		VulkanContext(GLFWwindow* windowHandle);

		void Init() override;
		void SwapBuffers() override;

	private:
		GLFWwindow* m_WindowHandle;

	};
}