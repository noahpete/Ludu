#pragma once

#include "Core/Window.h"
#include "Events/EventManager.h"

int main(int argc, char* argv[]);

namespace Ludu {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnWindowQuitEvent(QuitEvent& event);
		void OnWindowResizeEvent(WindowResizeEvent& event);

		static Application& Get() { return *s_Instance; }

	private:
		static Application* s_Instance;
		bool m_Running;

	};

	// To be defined in client
	Application* CreateApplication();
}