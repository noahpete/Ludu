#pragma once

namespace Ludu {

	class RendererAPI
	{
	public:
		enum class API { None, Vulkan };

		virtual ~RendererAPI() = default;

		virtual void Init() = 0;

		static API GetAPI() { return s_API; }
		static Scope<RendererAPI> Create();

	private:
		static API s_API;

	};
}