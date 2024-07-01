#include "ldpch.h"
#include "Application.h"


namespace Ludu
{
	Application *Application::s_Instance = nullptr;

	Application::Application()
		: m_Running(true)
	{
		m_Window = Window::Create(1280, 720, "Ludu");
		m_Renderer = Renderer::Create(m_Window);
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_Running)
		{
			m_Window->OnUpdate();

			if (m_Window->ShouldClose())
				m_Running = false;

			m_Renderer->OnUpdate();
		}

		m_Renderer->Shutdown();

	}
}
