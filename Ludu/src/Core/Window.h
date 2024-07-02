#pragma once

#include "ldpch.h"

#include "Core/Core.h"
#include "Events/Event.h"

namespace Ludu {

	class Window
	{
	public:
		Window() = default;
		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual bool ShouldClose() = 0;

		virtual void* GetNativeWindow() const = 0;

		static Scope<Window> Create(uint32_t width, uint32_t height, const std::string& title);

	};
}