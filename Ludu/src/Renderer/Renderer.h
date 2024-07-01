#pragma once

#include "Core/Core.h"
#include "Core/Window.h"

namespace Ludu {

    class Renderer
    {
    public:
        Renderer() = default;
        virtual ~Renderer() = default;

        virtual void OnUpdate() = 0;

        static Scope<Renderer> Create(Ref<Window> window);

    };
}