#include "ldpch.h"
#include "Application.h"


namespace Ludu
{
	Application *Application::s_Instance = nullptr;

	Application::Application()
		: m_Running(true)
	{

	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (true)
		{
			
		}

	}
}
