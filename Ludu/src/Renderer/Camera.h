#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include "Core/Timestep.h"

#include "Events/EventManager.h"

namespace Ludu {

	class Camera
	{
	public:
		Camera();

		void SetOrthographicProjection(float left, float right, float top, float bottom, float nearVal, float farVal);
		void SetPerspectiveProjection(float fovY, float aspect, float nearVal, float farVal);

		void SetViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up = { 0.0f, -1.0f, 0.0f });
		void SetViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up = { 0.0f, -1.0f, 0.0f });
		void SetViewYXZ(glm::vec3 position, glm::vec3 rotation);

		void OnWindowResizeEvent(WindowResizeEvent& event);

		const glm::mat4& GetProjection() const { return m_ProjectionMatrix; }
		const glm::mat4& GetView() const { return m_ViewMatrix; }

	private:
		glm::mat4 m_ProjectionMatrix{ 1.0f };
		glm::mat4 m_ViewMatrix{ 1.0f };

	};
}