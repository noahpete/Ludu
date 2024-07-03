#pragma once

#include "ldpch.h"
#include <glm/glm.hpp>
//#include "Renderer/Camera.h"
//#include "Scene/ScriptableEntity.h"

#include "Platform/Vulkan/VulkanModel.h"

namespace Ludu {

    struct TagComponent
    {
        std::string Tag;

        TagComponent() = default;
        TagComponent(const TagComponent&) = default;
        TagComponent(const std::string& tag)
            : Tag(tag) {}
    };

    struct TransformComponent
    {
        glm::vec2 Position;
        glm::vec2 Scale;
        float Rotation;

        glm::vec2 Translation2D{};
        glm::mat2 GetMat2() { return glm::mat2{1.0f}; }

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::vec2 position, glm::vec2 scale, float rotation)
            : Position(position), Scale(scale), Rotation(rotation) {}
    };

    struct SpriteRendererComponent
    {
        glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        SpriteRendererComponent(const glm::vec4& color)
            : Color(color) {}
    };

    struct MeshComponent
    {
        Ref<VulkanModel> Model;
        glm::vec3 Color = { 1.0f, 1.0f, 1.0f };

        MeshComponent(std::vector<Ludu::Vertex>& vertices)
        {
            Model = CreateRef<VulkanModel>(vertices);
        }
    };

    //struct CameraComponent
    //{
    //    Camera Camera;
    //    bool Primary = true;

    //    CameraComponent() = default;
    //    CameraComponent(const CameraComponent&) = default;
    //    // CameraComponent(const )
    //};

    //struct NativeScriptComponent
    //{
    //    ScriptableEntity* Instance = nullptr;


    //    ScriptableEntity* (*InstantiateScript)();
    //    void (*DestroyScript)(NativeScriptComponent*);

    //    template<typename T>
    //    void Bind()
    //    {
    //        InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
    //        DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
    //    }
    //};
}