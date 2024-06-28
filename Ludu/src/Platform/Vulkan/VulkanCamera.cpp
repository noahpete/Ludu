#include "VulkanCamera.h"

#include "Core/Core.h"

namespace Ludu
{

    void VulkanCamera::SetOrthographicProjection(float left, float right, float top, float bottom, float near, float far)
    {
        m_ProjectionMatrix = glm::mat4{1.0f};
        m_ProjectionMatrix[0][0] = 2.f / (right - left);
        m_ProjectionMatrix[1][1] = 2.f / (bottom - top);
        m_ProjectionMatrix[2][2] = 1.f / (far - near);
        m_ProjectionMatrix[3][0] = -(right + left) / (right - left);
        m_ProjectionMatrix[3][1] = -(bottom + top) / (bottom - top);
        m_ProjectionMatrix[3][2] = -near / (far - near);
    }

    void VulkanCamera::SetPerspectiveProjection(float fovY, float aspect, float near, float far)
    {
        LD_CORE_ASSERT(glm::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);

        const float tanHalfFovy = tan(fovY / 2.f);
        m_ProjectionMatrix = glm::mat4{0.0f};
        m_ProjectionMatrix[0][0] = 1.f / (aspect * tanHalfFovy);
        m_ProjectionMatrix[1][1] = 1.f / (tanHalfFovy);
        m_ProjectionMatrix[2][2] = far / (far - near);
        m_ProjectionMatrix[2][3] = 1.f;
        m_ProjectionMatrix[3][2] = -(far * near) / (far - near);
    }
}