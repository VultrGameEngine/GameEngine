#include <windows/component_window.h>

using namespace Vultr;

void register_component_window(Engine *e)
{
    void *state = static_cast<void *>(nullptr);
    WindowRenderer renderer = component_window_render;
    editor_register_window(e, component_window_render, state);
}

void component_window_render(Engine *e, const UpdateTick &tick, void *state)
{
    ImGui::Begin("Inspector");
    Entity entity = get_editor().selected_entity;
    if (entity != INVALID_ENTITY)
    {
        auto &registry = e->component_registry;
        component_registry_render_entity_components(e, registry, entity);
        if (ImGui::Button("Add Component"))
        {
            ImGui::OpenPopup("ComponentList");
        }
        if (ImGui::BeginPopup("ComponentList"))
        {
            for (auto [type, pair] : registry.components)
            {
                if (world_get_component_manager(get_current_world(e)).component_arrays[type]->HasData(entity))
                    continue;
                if (ImGui::MenuItem(component_registry_get_component_name(registry, type)))
                {
                    pair.component_constructor(e, entity);
                }
            }
            ImGui::EndPopup();
        }
    }

    ImGui::End();
}
