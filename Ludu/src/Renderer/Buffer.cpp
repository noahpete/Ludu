#include "ldpch.h"
#include "Buffer.h"

#include "Renderer/RendererAPI.h"
#include "Platform/Vulkan/VulkanBuffer.h"

namespace Ludu {

    Ref<VertexBuffer> VertexBuffer::Create(float vertices, uint32_t size)
    {
        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::None:
            return nullptr;

        //case RendererAPI::API::Vulkan:
        //    return CreateRef<VulkanVertexBuffer>(vertices, size);
        }

        return nullptr;
    }

    Ref<IndexBuffer> IndexBuffer::Create(float indices, uint32_t size)
    {
        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::None:
            return nullptr;

        //case RendererAPI::API::Vulkan:
        //    return CreateRef<VulkanIndexBuffer>(indices, size);
        }

        return nullptr;
    }

}