#include "Editor/ImGuiLayer.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_vulkan.h>

#include "Core/Application.h"
#include "Platform/Vulkan/VulkanRenderer.h"

namespace Ludu
{

    ImGuiLayer::ImGuiLayer()
        : Layer("EditorLayer")
    {
    }

    void ImGuiLayer::OnAttach()
    {
        Renderer::Get().InitImGui();
    }

    void ImGuiLayer::OnDetach()
    {
    }

    void ImGuiLayer::Begin()
    {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiLayer::End()
    {
        ImGuiIO& io = ImGui::GetIO();
        auto width = Application::Get().GetWindow().GetWidth();
        auto height = Application::Get().GetWindow().GetHeight();
        io.DisplaySize = ImVec2(width, height);

        ImGui::Render();
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), dynamic_cast<VulkanRenderer*>(&Renderer::Get())->GetCurrentCommandBuffer());
    }
}