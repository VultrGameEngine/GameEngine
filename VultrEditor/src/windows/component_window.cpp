#include <windows/component_window.h>

using namespace Vultr;

void register_component_window()
{
    void *state = static_cast<void *>(nullptr);
    WindowRenderer renderer = component_window_render;
    editor_register_window(component_window_render, state);
}

void component_window_render(const UpdateTick &tick, void *state)
{
    ImGui::Begin("Inspector");
    Entity entity = get_editor().selected_entity;
    if (entity != INVALID_ENTITY)
    {
        auto &registry = engine_global()->component_registry;
        component_registry_render_entity_components(registry, entity);
        if (ImGui::Button("Add Component"))
        {
            ImGui::OpenPopup("ComponentList");
        }
        if (ImGui::BeginPopup("ComponentList"))
        {
            for (auto [type, pair] : registry.components)
            {
                if (world_get_component_manager(get_current_world()).component_arrays[type]->HasData(entity))
                    continue;
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
