#include "ldpch.h"
#include "Application.h"

#include "Renderer/Camera.h"
#include "Core/Input.h"

namespace Ludu
{
	Application *Application::s_Instance = nullptr;

	Application::Application()
		: m_Running(true)
	{
		LD_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = Window::Create(1280, 720, "Ludu");
		m_Renderer = Renderer::Create(m_Window);
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		Camera defaultCamera{};

		while (m_Running)
		{
			Timestep ts = 1.0f / 60.0f;

			m_Window->OnUpdate();

			if (m_Window->ShouldClose())
				m_Running = false;

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(ts);

			defaultCamera.SetPerspectiveProjection(glm::radians(50.0f), m_Renderer->GetAspectRatio(), 0.1f, 10.0f);

			m_Renderer->OnUpdate(defaultCamera);
		}

		m_Renderer->Shutdown();

	}

    void Application::PushLayer(Layer *layer)
    {
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
    }

	void Application::PushOverlay(Layer *layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}
}
