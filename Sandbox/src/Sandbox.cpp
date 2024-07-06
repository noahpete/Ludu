#include <Ludu.h>

#include <iostream>

class ExampleLayer : public Ludu::Layer
{
public:
	ExampleLayer()
	{
		m_Entity = m_Scene.CreateEntity("Example");

        //std::vector<Ludu::Vertex> vertices{
        //    // left face (white)
        //    {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
        //    {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
        //    {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
        //    {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},

        //    // right face (yellow)
        //    {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
        //    {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
        //    {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
        //    {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},

        //    // top face (orange, remember y axis points down)
        //    {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
        //    {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
        //    {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
        //    {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},

        //    // bottom face (red)
        //    {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
        //    {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
        //    {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
        //    {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},

        //    // nose face (blue)
        //    {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
        //    {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
        //    {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
        //    {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},

        //    // tail face (green)
        //    {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
        //    {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
        //    {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
        //    {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
        //};

        //std::vector<uint32_t> indices{ 0,  1,  2,  0,  3,  1,  4,  5,  6,  4,  7,  5,  8,  9,  10, 8,  11, 9,
        //                  12, 13, 14, 12, 15, 13, 16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21 };

        m_Entity.AddComponent<Ludu::MeshComponent>("smooth_vase.obj");
        m_Entity.GetComponent<Ludu::TransformComponent>().Translation = { 0.0f, 0.0f, 2.5f };
        m_Entity.GetComponent<Ludu::TransformComponent>().Scale = { 0.5f, 0.5f, 0.5f };

	}

	~ExampleLayer()
	{
	}

	void OnUpdate(Ludu::Timestep ts) override
	{
		Ludu::Renderer::Get().Submit(&m_Entity);
        m_Entity.GetComponent<Ludu::TransformComponent>().Rotation.y += 1.0f * ts;
        m_Entity.GetComponent<Ludu::TransformComponent>().Rotation.x += 0.5f * ts;
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