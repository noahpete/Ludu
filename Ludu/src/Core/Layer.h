#pragma once

#include "ldpch.h"
#include "Core/Core.h"
#include "Core/Timestep.h"
#include "Events/Event.h"

namespace Ludu {
    class Layer {
    public:
        Layer(const std::string& name = "Layer")
            : m_DebugName(name)
        {
        }

        virtual ~Layer() = default;

        virtual void OnAttach() {};
        virtual void OnDetach() {};
        virtual void OnUpdate(Timestep ts) {};
        virtual void OnImGuiRender() {};

        const std::string& GetName() const { return m_DebugName; }

    protected:
        std::string m_DebugName;
    };
} // namespace Ludu