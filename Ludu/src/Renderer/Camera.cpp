#include "ldpch.h"
#include "Camera.h"

namespace Ludu {

	void Camera::SetOrthographicProjection(float left, float right, float top, float bottom, float nearVal, float farVal)
	{
		m_ProjectionMatrix = glm::mat4{ 1.0f };
		m_ProjectionMatrix[0][0] = 2.f / (right - left);
		m_ProjectionMatrix[1][1] = 2.f / (bottom - top);
		m_ProjectionMatrix[2][2] = 1.f / (farVal - nearVal);
		m_ProjectionMatrix[3][0] = -(right + left) / (right - left);
		m_ProjectionMatrix[3][1] = -(bottom + top) / (bottom - top);
		m_ProjectionMatrix[3][2] = -nearVal / (farVal - nearVal);
	}

	void Camera::SetPerspectiveProjection(float fovY, float aspect, float nearVal, float farVal)
	{
		const float tanHalfFovy = tan(fovY / 2.f);
		m_ProjectionMatrix = glm::mat4{ 0.0f };
		m_ProjectionMatrix[0][0] = 1.f / (aspect * tanHalfFovy);
		m_ProjectionMatrix[1][1] = 1.f / (tanHalfFovy);
		m_ProjectionMatrix[2][2] = farVal / (farVal - nearVal);
		m_ProjectionMatrix[2][3] = 1.f;
		m_ProjectionMatrix[3][2] = -(farVal * nearVal) / (farVal - nearVal);
	}
}
