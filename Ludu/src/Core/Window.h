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

	};
}