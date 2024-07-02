#pragma once

#include "Renderer/Buffer.h"

namespace Ludu {

	class VulkanVertexBuffer : public VertexBuffer
	{
	public:
		~VulkanVertexBuffer() {}

		void Bind() const override;
		void Unbind() const override;

	};

	class VulkanIndexBuffer : public IndexBuffer
	{
	public:
		~VulkanIndexBuffer() {}

		void Bind() const override;
		void Unbind() const override;

	};
}