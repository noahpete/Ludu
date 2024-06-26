#pragma once

#include "Core/Core.h"

extern Ludu::Application* Ludu::CreateApplication();

int main(int argc, char* argv[])
{
	Ludu::Log::Init();

	auto app = Ludu::CreateApplication();
	app->Run();
	delete app;

	return 0;
}