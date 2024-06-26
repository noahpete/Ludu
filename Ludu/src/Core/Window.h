#pragma once

#include "ldpch.h"

#include "Core/Core.h"
#include "Events/Event.h"

namespace Ludu {

	class Window
	{
	public:
		Window() {}
		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		static Window* Create(const std::string& title = "Ludu Engine", uint32_t width = 1280, uint32_t height = 720);
	};
}