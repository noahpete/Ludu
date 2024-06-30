#include "Application.h"

#include "Platform/Vulkan/VulkanSimpleRenderSystem.h"
#include "Platform/Vulkan/VulkanCamera.h"
#include "Platform/Vulkan/VulkanController.h"
#include "Platform/Vulkan/VulkanBuffer.h"
#include "Platform/Vulkan/VulkanDescriptors.h"

namespace Ludu
{
	struct GlobalUBO
	{
		glm::mat4 projectionView{ 1.0f };
		glm::vec3 lightDirection = glm::normalize(glm::vec3{ 1.0f, -3.0f, -1.0f });
	};

	Application *Application::s_Instance = nullptr;

	Application::Application()
		: m_Running(true)
	{
		m_GlobalPool = VulkanDescriptorPool::Builder(m_Device).setMaxSets(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT).addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VulkanSwapChain::MAX_FRAMES_IN_FLIGHT).build();
		LoadGameObjects();
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		std::vector<std::unique_ptr<VulkanBuffer>> uboBuffers(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT);

		for (int i = 0; i < uboBuffers.size(); i++)
		{
			uboBuffers[i] = std::make_unique<VulkanBuffer>(
				m_Device,
				sizeof(GlobalUBO),
				1,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
			);
			uboBuffers[i]->map();
		}

		auto globalSetLayout = VulkanDescriptorSetLayout::Builder(m_Device).addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT).build();

		std::vector<VkDescriptorSet> globalDescriptorSets(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT);
		for (int i = 0; i < globalDescriptorSets.size(); i++)
		{
			auto bufferInfo = uboBuffers[i]->descriptorInfo();
			VulkanDescriptorWriter(*globalSetLayout, *m_GlobalPool).writeBuffer(0, &bufferInfo).build(globalDescriptorSets[i]);
		}

		VulkanSimpleRenderSystem simpleRenderSystem{ m_Device, m_Renderer.GetSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout() };

		VulkanCamera camera{};

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
				int frameIndex = m_Renderer.GetFrameIndex();
				FrameInfo frameInfo{ frameIndex, frameTime, commandBuffer, camera, globalDescriptorSets[frameIndex] };

				// Update
				GlobalUBO ubo{};
				ubo.projectionView = camera.GetProjection() * camera.GetView();
				uboBuffers[frameIndex]->writeToBuffer(&ubo);
				uboBuffers[frameIndex]->flush();
				 
				// Render
				m_Renderer.BeginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.RenderGameObjects(frameInfo, m_GameObjects);
				m_Renderer.EndSwapChainRenderPass(commandBuffer);
				m_Renderer.EndFrame();
			}
		}

		vkDeviceWaitIdle(m_Device.device());
	}

	void Application::LoadGameObjects()
	{
		std::shared_ptr<VulkanModel> model = VulkanModel::CreateModelFromFile(m_Device, "flat_vase.obj");

		auto gameObject = VulkanGameObject::CreateGameObject();

		gameObject.model = model;
		gameObject.transform.Translation = {0.0f, 0.5f, 2.5f};
		gameObject.transform.Scale = {1.0f, 1.0f, 1.0f};

		m_GameObjects.push_back(std::move(gameObject));
	}
}
