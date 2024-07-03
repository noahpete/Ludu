#include <Ludu.h>

#include <iostream>

class ExampleLayer : public Ludu::Layer
{
public:
	ExampleLayer()
	{
		m_Entity = m_Scene.CreateEntity("Example");

		std::vector<Ludu::Vertex> vertices{
			{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
			{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};

		m_Entity.AddComponent<Ludu::MeshComponent>(vertices);
		m_Entity.GetComponent<Ludu::MeshComponent>().Color = { 0.2f, 0.3f, 0.8f };
		m_Entity.GetComponent<Ludu::TransformComponent>().Scale2D = { 2.0f, 0.5f };
	}

	~ExampleLayer()
	{
	}

	void OnUpdate(Ludu::Timestep ts) override
	{
		Ludu::Renderer::Get().Submit(&m_Entity);
	}

private:
	Ludu::Scene m_Scene;
	Ludu::Entity m_Entity;
};

class Sandbox : public Ludu::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{
	}
};

Ludu::Application *Ludu::CreateApplication()
{
	return new Sandbox();
}