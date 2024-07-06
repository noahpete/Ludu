#include "ldpch.h"
#include "Application.h"

#include "Renderer/Camera.h"

#include "Core/Input.h"
#include "Core/Util.h"

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

		m_ImGuiLayer = new ImGuiLayer();
		PushLayer(m_ImGuiLayer);

		m_EditorLayer = new EditorLayer();
		PushLayer(m_EditorLayer);

		m_LastFrameTime = Util::GetTime();
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_Running)
		{
			// Update

			float curTime = Util::GetTime();
			Timestep ts = curTime - m_LastFrameTime;
			m_LastFrameTime = curTime;
			LD_CORE_INFO("{0}", float(ts));
			m_Window->OnUpdate();

			if (m_Window->ShouldClose())
				m_Running = false;

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(ts);

			// Render

			m_Renderer->Begin();

			auto camera = GetPrimaryCamera();
			camera->SetPerspectiveProjection(glm::radians(50.0f), m_Renderer->GetAspectRatio(), 0.1f, 10.0f);
			 
			m_Renderer->OnUpdate(*camera);

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Renderer->End();
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

    Ref<Camera> Application::GetPrimaryCamera()
    {
		// For now always returns editor camera
        return m_EditorLayer->GetEditorCamera();
    }
}
