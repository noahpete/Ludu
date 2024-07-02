#include "ldpch.h"
#include "RendererAPI.h"

#include "Platform/Vulkan/VulkanRenderer.h"

namespace Ludu {

    RendererAPI::API RendererAPI::s_API = API::Vulkan;

    Scope<RendererAPI> RendererAPI::Create()
    {
        switch (s_API)
        {
        case Ludu::RendererAPI::API::None:
            return nullptr;
        /*case Ludu::RendererAPI::API::Vulkan:
            return CreateScope<VulkanRenderer>();*/
        default:
            LD_CORE_ASSERT(false, "Unknown RendererAPI!");
            return nullptr;
        }
    }

}