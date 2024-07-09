#include <Ludu.h>

#include <iostream>

class ExampleLayer : public Ludu::Layer
{
public:
	ExampleLayer()
	{
		m_Entity = m_Scene.CreateEntity("Example");
		m_SecondEntity = m_Scene.CreateEntity("Example2");
		m_FloorEntity = m_Scene.CreateEntity("Floor");

        m_Entity.AddComponent<Ludu::MeshComponent>("../flat_vase.obj");
        m_Entity.GetComponent<Ludu::TransformComponent>().Translation = { -0.3f, 0.2f, 0.5f };
        m_Entity.GetComponent<Ludu::TransformComponent>().Scale = { 1.0f, 1.0f, 1.0f };

        m_SecondEntity.AddComponent<Ludu::MeshComponent>("../flat_vase.obj");
        m_SecondEntity.GetComponent<Ludu::TransformComponent>().Translation = { 0.3f, 0.2f, 0.5f };
        m_SecondEntity.GetComponent<Ludu::TransformComponent>().Scale = { 2.0f, 0.7f, 1.0f };

		m_FloorEntity.AddComponent<Ludu::MeshComponent>("../quad.obj");
		m_FloorEntity.GetComponent<Ludu::TransformComponent>().Translation = { 0.0f, 0.2f, 0.0f };
		m_FloorEntity.GetComponent<Ludu::TransformComponent>().Scale = { 3.0f, 1.0f, 3.0f };
	}

	~ExampleLayer()
	{
	}

	void OnUpdate(Ludu::Timestep ts) override
	{
		Ludu::Renderer::Get().Submit(&m_Entity);
		Ludu::Renderer::Get().Submit(&m_SecondEntity);
		Ludu::Renderer::Get().Submit(&m_FloorEntity);
        /*m_Entity.GetComponent<Ludu::TransformComponent>().Rotation.y += 1.0f * ts;
        m_Entity.GetComponent<Ludu::TransformComponent>().Rotation.x += 0.5f * ts;*/
	}

private:
	Ludu::Scene m_Scene;
	Ludu::Entity m_Entity;
    Ludu::Entity m_SecondEntity;
	Ludu::Entity m_FloorEntity;
	Ludu::Entity m_SpriteEntity;
	
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