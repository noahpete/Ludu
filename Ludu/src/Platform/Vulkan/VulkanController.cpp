#include "ldpch.h"
#include "Platform/Vulkan/VulkanController.h"

#include <limits>

namespace Ludu {

	void VulkanController::MoveInPlaneXZ(GLFWwindow* window, float dt, VulkanGameObject& gameObject)
	{
		glm::vec3 rotate{ 0.0f };
		if (glfwGetKey(window, m_Keys.lookRight) == GLFW_PRESS) rotate.y += 1.0f;
		if (glfwGetKey(window, m_Keys.lookLeft) == GLFW_PRESS) rotate.y -= 1.0f;
		if (glfwGetKey(window, m_Keys.lookUp) == GLFW_PRESS) rotate.x += 1.0f;
		if (glfwGetKey(window, m_Keys.lookDown) == GLFW_PRESS) rotate.x -= 1.0f;

		if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon())
			gameObject.transform.Rotation += m_LookSpeed * dt * glm::normalize(rotate);

		gameObject.transform.Rotation.x = glm::clamp(gameObject.transform.Rotation.x, -1.5f, 1.5f);
		gameObject.transform.Rotation.y = glm::mod(gameObject.transform.Rotation.y, glm::two_pi<float>());

		float yaw = gameObject.transform.Rotation.y;
		const glm::vec3 forwardDir{ sin(yaw), 0.0f, cos(yaw) };
		const glm::vec3 rightDir{ forwardDir.z, 0.0f, -forwardDir.x };
		const glm::vec3 upDir{ 0.0f, -1.0f, 0.0f };

		glm::vec3 moveDir{ 0.0f };
		if (glfwGetKey(window, m_Keys.forward) == GLFW_PRESS) moveDir += forwardDir;
		if (glfwGetKey(window, m_Keys.backward) == GLFW_PRESS) moveDir -= forwardDir;
		if (glfwGetKey(window, m_Keys.right) == GLFW_PRESS) moveDir += rightDir;
		if (glfwGetKey(window, m_Keys.left) == GLFW_PRESS) moveDir -= rightDir;
		if (glfwGetKey(window, m_Keys.up) == GLFW_PRESS) moveDir += upDir;
		if (glfwGetKey(window, m_Keys.down) == GLFW_PRESS) moveDir -= upDir;

		if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon())
			gameObject.transform.Translation += m_MoveSpeed * dt * glm::normalize(moveDir);

	}
}
