#include "Application.h"

#include "Platform/Vulkan/VulkanSimpleRenderSystem.h"

namespace Ludu {

	Application* Application::s_Instance = nullptr;

	Application::Application()
		: m_Running(true)
	{
		LoadGameObjects();
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		VulkanSimpleRenderSystem simpleRenderSystem{m_Device, m_Renderer.GetSwapChainRenderPass()};

		while (!m_Window.shouldClose())
		{
			glfwPollEvents();

			if (auto commandBuffer = m_Renderer.BeginFrame())
			{
				m_Renderer.BeginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.RenderGameObjects(commandBuffer, m_GameObjects);
				m_Renderer.EndSwapChainRenderPass(commandBuffer);
				m_Renderer.EndFrame();
			}
		}

		vkDeviceWaitIdle(m_Device.device());
	}

	void Application::LoadGameObjects()
	{
		std::vector<VulkanModel::Vertex> vertices{
			{ { 0.0f, -0.5f }, { 1.0f, 0.0f, 0.0f} },
			{ { 0.5f,  0.5f }, { 0.0f, 1.0f, 0.0f} },
			{ {-0.5f,  0.5f }, { 0.0f, 0.0f, 1.0f} }
		};

		auto model = std::make_shared<VulkanModel>(m_Device, vertices);
		auto triangle = VulkanGameObject::CreateGameObject();

		triangle.model = model;
		triangle.color = { 0.1f, 0.8f, 0.1f };
		triangle.transform2D.Translation.x = 0.2f;
		triangle.transform2D.Scale = { 2.0f, 0.5f };
		triangle.transform2D.rotation = 0.25f * glm::two_pi<float>();

		m_GameObjects.push_back(std::move(triangle));
	}
}
