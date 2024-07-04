#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace Ludu {

	class Camera
	{
	public:
		void SetOrthographicProjection(float left, float right, float top, float bottom, float nearVal, float farVal);
		void SetPerspectiveProjection(float fovY, float aspect, float nearVal, float farVal);

		const glm::mat4& GetProjection() const { return m_ProjectionMatrix; }

	private:
		glm::mat4 m_ProjectionMatrix{ 1.0f };
	};
}