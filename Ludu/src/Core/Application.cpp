#include "Application.h"
#include "Application.h"
#include "Application.h"
#include "Application.h"
#include "Application.h"
#include "Application.h"
#include "Application.h"

#include "Platform/Vulkan/VulkanWindow.h"


namespace Ludu {

	Application* Application::s_Instance = nullptr;

	Application::Application()
		: m_Running(true)
	{
		m_Window = std::unique_ptr<Window>(Window::Create());

		// Set callback(s)
		EventManager::Subscribe<Application, QuitEvent>(this, &Application::OnWindowQuitEvent);
		EventManager::Subscribe<Application, WindowResizeEvent>(this, &Application::OnWindowResizeEvent);
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_Running)
		{
			m_Window->OnUpdate();
		}
	}

	void Application::OnWindowQuitEvent(QuitEvent& event)
	{

	}

	void Application::OnWindowResizeEvent(WindowResizeEvent& event)
	{
		LD_CORE_INFO("WindowResizeEvent! {0}, {1}", event.m_Width, event.m_Height);
	}

}
