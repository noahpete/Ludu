#include "Core/Window.h"
#include "Platform/GeneralWindow.h"

namespace Ludu {

	static bool s_GLFWInitialized = false;

	Window* Window::Create(const std::string& title, uint32_t width, uint32_t height)
	{
		return new GeneralWindow(title, width, height);
	}

	GeneralWindow::GeneralWindow(const std::string& title, uint32_t width, uint32_t height)
	{
		Init(title, width, height);
	}

	GeneralWindow::~GeneralWindow()
	{
		Shutdown();
	}

	void GeneralWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void GeneralWindow::Init(const std::string& title, uint32_t width, uint32_t height)
	{
		m_Title = title;
		m_Width = width;
		m_Height = height;

		LD_CORE_INFO("Creating window {0} ({1}, {2})", title, width, height);

		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			LD_CORE_ASSERT(success, "Could not initialize GLFW!");
			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		//glfwSetWindowUserPointer(m_Window, m_Data);
		glfwSwapInterval(1);
	}

	void GeneralWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

}