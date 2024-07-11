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

        glm::mat3 GetNormalMatrix()
        {
            const float c3 = glm::cos(Rotation.z);
            const float s3 = glm::sin(Rotation.z);
            const float c2 = glm::cos(Rotation.x);
            const float s2 = glm::sin(Rotation.x);
            const float c1 = glm::cos(Rotation.y);
            const float s1 = glm::sin(Rotation.y);
            const glm::vec3 invScale = 1.0f / Scale;

            return glm::mat3{
                {
                    invScale.x * (c1 * c3 + s1 * s2 * s3),
                    invScale.x * (c2 * s3),
                    invScale.x * (c1 * s2 * s3 - c3 * s1),
                },
                {
                    invScale.y * (c3 * s1 * s2 - c1 * s3),
                    invScale.y * (c2 * c3),
                    invScale.y * (c1 * c3 * s2 + s1 * s3),
                },
                {
                    invScale.z * (c2 * s1),
                    invScale.z * (-s2),
                    invScale.z * (c1 * c2),
                },
            };
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
        Ref<VulkanModel> Model; // TODO: abstract into Model class
        glm::vec3 Color = {1.0f, 1.0f, 1.0f};

        MeshComponent(std::vector<Ludu::Vertex> &vertices, std::vector<uint32_t> &indices)
        {
            Model = CreateRef<VulkanModel>(vertices, indices);
        }

        MeshComponent(const std::string& modelFilePath)
        {
            Model = VulkanModel::CreateModelFromFile(modelFilePath);
        }
    };

    struct Texture2DComponent
    {

        Texture2DComponent(const std::string& texFilepath)
        {

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