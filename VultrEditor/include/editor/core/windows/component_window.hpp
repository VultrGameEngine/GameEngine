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
            if (entity >= 0)
            {
                auto &registry = engine_global().component_registry;
                component_registry_render_entity_components(registry, Editor::Get()->selected_entity);
                if (ImGui::Button("Add Component"))
                {
                    ImGui::OpenPopup("ComponentList");
                }
                if (ImGui::BeginPopup("ComponentList"))
                {
                    for (auto [type, pair] : registry.components)
                    {
                        if (ImGui::MenuItem(component_registry_get_component_name(registry, type)))
                        {
                            pair.component_constructor(entity);
                        }
                    }
                    ImGui::EndPopup();
                }
            }

            ImGui::End();
        }
    };
} // namespace Vultr::Editor
