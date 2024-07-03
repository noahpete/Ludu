#pragma once

#include "Core/Core.h"
#include "Core/Window.h"

#include "Scene/Entity.h"

namespace Ludu {

    class Renderer
    {
    public:
        Renderer() = default;
        virtual ~Renderer() = default;

        virtual void OnUpdate() = 0;
        virtual void Shutdown() = 0;

        virtual void Submit(Entity* entity) = 0;

        static Scope<Renderer> Create(Ref<Window> window);
        static Renderer& Get() { return *s_Instance; }

    protected:
        static Renderer* s_Instance;

    };
}