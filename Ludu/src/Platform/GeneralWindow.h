#pragma once

#include "Core/Window.h"

#include <GLFW/glfw3.h>

namespace Ludu {

	class GeneralWindow : public Window
	{
	public:
		GeneralWindow(const std::string& title, uint32_t width, uint32_t height);
		virtual ~GeneralWindow();

		void OnUpdate() override;

		inline uint32_t GetWidth() const override { return m_Width; }
		inline uint32_t GetHeight() const override { return m_Height; }

	private:
		GLFWwindow* m_Window;
		std::string m_Title;
		uint32_t m_Width, m_Height;

		void Init(const std::string& title, uint32_t width, uint32_t height);
		void Shutdown();

	};
}