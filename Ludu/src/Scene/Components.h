#pragma once

#include "ldpch.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// #include "Renderer/Camera.h"
// #include "Scene/ScriptableEntity.h"

#include "Platform/Vulkan/VulkanModel.h"

namespace Ludu
{

    struct TagComponent
    {
        std::string Tag;

        TagComponent() = default;
        TagComponent(const TagComponent &) = default;
        TagComponent(const std::string &tag)
            : Tag(tag) {}
    };

    struct TransformComponent
    {
        glm::vec3 Translation{ 0.0f };
        glm::vec3 Rotation{ 0.0f };
        glm::vec3 Scale{ 1.0f };

        glm::mat4 GetTransform()
        {
            auto transform = glm::translate(glm::mat4{ 1.0f }, Translation);
            transform = glm::rotate(transform, Rotation.x, { 1.0f, 0.0f, 0.0f });
            transform = glm::rotate(transform, Rotation.y, { 0.0f, 1.0f, 0.0f });
            transform = glm::rotate(transform, Rotation.z, { 0.0f, 0.0f, 1.0f });
            transform = glm::scale(transform, Scale);
            return transform;
        }

        TransformComponent() = default;
        TransformComponent(const TransformComponent &) = default;
        TransformComponent(const glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale)
            : Translation(translation), Rotation(rotation), Scale(scale) {}
    };

    struct SpriteRendererComponent
    {
        glm::vec4 Color{1.0f, 1.0f, 1.0f, 1.0f};

        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent &) = default;
        SpriteRendererComponent(const glm::vec4 &color)
            : Color(color) {}
    };

    struct MeshComponent
    {
        Ref<VulkanModel> Model;
        glm::vec3 Color = {1.0f, 1.0f, 1.0f};

        MeshComponent(std::vector<Ludu::Vertex> &vertices)
        {
            Model = CreateRef<VulkanModel>(vertices);
        }
    };

    // struct CameraComponent
    //{
    //     Camera Camera;
    //     bool Primary = true;

    //    CameraComponent() = default;
    //    CameraComponent(const CameraComponent&) = default;
    //    // CameraComponent(const )
    //};

    // struct NativeScriptComponent
    //{
    //     ScriptableEntity* Instance = nullptr;

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