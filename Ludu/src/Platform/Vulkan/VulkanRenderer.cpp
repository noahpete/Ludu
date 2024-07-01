#include "ldpch.h"
#include "VulkanRenderer.h"

namespace Ludu
{

    VulkanRenderer::VulkanRenderer(Ref<VulkanWindow> window)
        : m_Device{*window},
          m_Pipeline{m_Device, VulkanPipeline::DefaultPipelineConfigInfo(window->GetWidth(), window->GetHeight())}
    {
    }

    VulkanRenderer::~VulkanRenderer()
    {
    }

    void VulkanRenderer::OnUpdate()
    {
    }
}