#include "Application.h"

#include "Platform/Vulkan/VulkanSimpleRenderSystem.h"
#include "Platform/Vulkan/VulkanCamera.h"
#include "Platform/Vulkan/VulkanController.h"

namespace Ludu
{

	Application *Application::s_Instance = nullptr;

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
		VulkanCamera camera{};
		// camera.SetViewDirection(glm::vec3(0.0f), glm::vec3(0.5f, 0.0f, 1.0f));
		// camera.SetViewTarget(glm::vec3(-1.0f, -2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 2.5f));

		auto viewerObject = VulkanGameObject::CreateGameObject();
		VulkanController cameraController{};

		auto currentTime = std::chrono::high_resolution_clock::now();

		while (!m_Window.shouldClose())
		{
			glfwPollEvents();

			auto newTime = std::chrono::high_resolution_clock::now();
			float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
			currentTime = newTime;

			//frameTime = glm::min(frameTime, MAX_FRAME_TIME);

			cameraController.MoveInPlaneXZ(m_Window.GetGLFWWindow(), frameTime, viewerObject);
			camera.SetViewXYZ(viewerObject.transform.Translation, viewerObject.transform.Rotation);

			float aspect = m_Renderer.GetAspectRatio();
			// camera.SetOrthographicProjection(-aspect, aspect, -1, 1, -1, 1);
			camera.SetPerspectiveProjection(glm::radians(50.0f), aspect, 0.1f, 10.0f);

			if (auto commandBuffer = m_Renderer.BeginFrame())
			{
				m_Renderer.BeginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.RenderGameObjects(commandBuffer, m_GameObjects, camera);
				m_Renderer.EndSwapChainRenderPass(commandBuffer);
				m_Renderer.EndFrame();
			}
		}

		vkDeviceWaitIdle(m_Device.device());
	}

	std::unique_ptr<VulkanModel> CreateCubeModel(VulkanDevice &device, glm::vec3 offset)
	{
		VulkanModel::Builder modelBuilder{};
		modelBuilder.vertices = {
			// left face (white)
			{{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
			{{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
			{{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
			{{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},

			// right face (yellow)
			{{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .8f, .1f}},
			{{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
			{{.5f, .5f, -.5f}, {.8f, .8f, .1f}},

			// top face (orange, remember y axis points down)
			{{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
			{{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
			{{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
			{{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},

			// bottom face (red)
			{{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .1f, .1f}},
			{{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
			{{.5f, .5f, -.5f}, {.8f, .1f, .1f}},

			// nose face (blue)
			{{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
			{{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
			{{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
			{{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},

			// tail face (green)
			{{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
			{{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
			{{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
			{{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
		};
		for (auto& v : modelBuilder.vertices) {
			v.position += offset;
		}

		modelBuilder.indices = { 0,  1,  2,  0,  3,  1,  4,  5,  6,  4,  7,  5,  8,  9,  10, 8,  11, 9,
								12, 13, 14, 12, 15, 13, 16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21 };

		return std::make_unique<VulkanModel>(device, modelBuilder);
	}

	void Application::LoadGameObjects()
	{
		std::shared_ptr<VulkanModel> model = CreateCubeModel(m_Device, {0.0f, 0.0f, 0.0f});

		auto cube = VulkanGameObject::CreateGameObject();

		cube.model = model;
		cube.transform.Translation = {0.0f, 0.0f, 2.5f};
		cube.transform.Scale = {0.5f, 0.5f, 0.5f};

		m_GameObjects.push_back(std::move(cube));
	}
}
