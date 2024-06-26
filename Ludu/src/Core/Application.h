#pragma once

int main(int argc, char* argv[]);

namespace Ludu {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		static Application& Get() { return *s_Instance; }

	private:
		static Application* s_Instance;

	};

	// To be defined in client
	Application* CreateApplication();
}