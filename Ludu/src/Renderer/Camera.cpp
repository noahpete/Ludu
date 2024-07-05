#include "Camera.h"
#include "Camera.h"
#include "Camera.h"
#include "Camera.h"
#include "Camera.h"
#include "ldpch.h"

#include "Renderer/Renderer.h"

namespace Ludu {
	Camera::Camera()
	{
		/*SetPerspectiveProjection(glm::radians(50.0f), Renderer::Get().GetAspectRatio(), 0.1f, 10.0f);
		EventManager::Subscribe<Camera, WindowResizeEvent>(this, &Camera::OnWindowResizeEvent);*/
	}

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

	void Camera::SetViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up)
	{
		const glm::vec3 w{ glm::normalize(direction) };
		const glm::vec3 u{ glm::normalize(glm::cross(w, up)) };
		const glm::vec3 v{ glm::cross(w, u) };

		m_ViewMatrix = glm::mat4{ 1.f };
		m_ViewMatrix[0][0] = u.x;
		m_ViewMatrix[1][0] = u.y;
		m_ViewMatrix[2][0] = u.z;
		m_ViewMatrix[0][1] = v.x;
		m_ViewMatrix[1][1] = v.y;
		m_ViewMatrix[2][1] = v.z;
		m_ViewMatrix[0][2] = w.x;
		m_ViewMatrix[1][2] = w.y;
		m_ViewMatrix[2][2] = w.z;
		m_ViewMatrix[3][0] = -glm::dot(u, position);
		m_ViewMatrix[3][1] = -glm::dot(v, position);
		m_ViewMatrix[3][2] = -glm::dot(w, position);
	}

	void Camera::SetViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up)
	{
		SetViewDirection(position, target - position, up);
	}

	void Camera::SetViewYXZ(glm::vec3 position, glm::vec3 rotation)
	{
		const float c3 = glm::cos(rotation.z);
		const float s3 = glm::sin(rotation.z);
		const float c2 = glm::cos(rotation.x);
		const float s2 = glm::sin(rotation.x);
		const float c1 = glm::cos(rotation.y);
		const float s1 = glm::sin(rotation.y);
		const glm::vec3 u{ (c1 * c3 + s1 * s2 * s3), (c2 * s3), (c1 * s2 * s3 - c3 * s1) };
		const glm::vec3 v{ (c3 * s1 * s2 - c1 * s3), (c2 * c3), (c1 * c3 * s2 + s1 * s3) };
		const glm::vec3 w{ (c2 * s1), (-s2), (c1 * c2) };
		m_ViewMatrix = glm::mat4{ 1.f };
		m_ViewMatrix[0][0] = u.x;
		m_ViewMatrix[1][0] = u.y;
		m_ViewMatrix[2][0] = u.z;
		m_ViewMatrix[0][1] = v.x;
		m_ViewMatrix[1][1] = v.y;
		m_ViewMatrix[2][1] = v.z;
		m_ViewMatrix[0][2] = w.x;
		m_ViewMatrix[1][2] = w.y;
		m_ViewMatrix[2][2] = w.z;
		m_ViewMatrix[3][0] = -glm::dot(u, position);
		m_ViewMatrix[3][1] = -glm::dot(v, position);
		m_ViewMatrix[3][2] = -glm::dot(w, position);
	}

	void Ludu::Camera::OnWindowResizeEvent(WindowResizeEvent& event)
	{
		//SetPerspectiveProjection(glm::radians(50.0f), Renderer::Get().GetAspectRatio(), 0.1f, 10.0f);
	}
}
