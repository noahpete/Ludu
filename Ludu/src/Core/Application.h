#pragma once

#include "Core/Window.h"
#include "Core/Layer.h"
#include "Core/LayerStack.h"

#include "Events/EventManager.h"
#include "Renderer/Renderer.h"

int main(int argc, char* argv[]);

namespace Ludu {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		void OnWindowQuitEvent(QuitEvent& event);
		void OnWindowResizeEvent(WindowResizeEvent& event);

		Window& GetWindow() { return *m_Window; }
		
		static Application& Get() { return *s_Instance; }

	private:
		static Application* s_Instance;
		bool m_Running;

		LayerStack m_LayerStack;

		Ref<Window> m_Window;
		Ref<Renderer> m_Renderer;

	};

	// To be defined in client
	Application* CreateApplication();
}