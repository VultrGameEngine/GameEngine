#pragma once
#include <editor/imgui/window.hpp>
#include <imgui/imgui.h>
#include <editor/core/windows/component_window.hpp>
#include <editor/editor.hpp>
#include <core/component_renderer.h>
#include <ecs/world/world.hpp>
#include <engine.hpp>

namespace Vultr::Editor
{
class ComponentWindow : public Window
{
  public:
    void Render() override
    {
        ImGui::Begin("Inspector");
        Entity entity = Editor::Get()->selected_entity;
        if (entity.id >= 0)
        {
            Vultr::Engine::GetComponentRegistry().RenderEntityComponents(
                Editor::Get()->selected_entity);
            if (ImGui::Button("Add Component"))
            {
                ImGui::OpenPopup("ComponentList");
            }
            if (ImGui::BeginPopup("ComponentList"))
            {
                for (auto [name, constructor] : Vultr::Engine::GetComponentRegistry()
                                                    .GetComponentConstructors())
                {
                    if (ImGui::MenuItem(name))
                    {
                        constructor(entity);
                    }
                }
                ImGui::EndPopup();
            }
        }

        ImGui::End();
    }
};
} // namespace Vultr::Editor
