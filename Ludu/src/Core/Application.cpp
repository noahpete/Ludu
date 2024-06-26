#include "Application.h"

#include "Core/Log.h"

namespace Ludu {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		LD_CORE_TRACE("Ludu::Application ctor!");
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
	}

}
