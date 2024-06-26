#include <Ludu.h>

#include <iostream>

class Sandbox : public Ludu::Application
{
public:
	Sandbox()
	{
	}

	~Sandbox()
	{

	}

};

Ludu::Application* Ludu::CreateApplication()
{
	return new Sandbox();
}