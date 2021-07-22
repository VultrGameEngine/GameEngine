#include <windows/entity_window.h>

using namespace Vultr;
void register_entity_window(Vultr::Engine *e, Editor *editor)
{
    void *state = static_cast<void *>(nullptr);
    WindowRenderer renderer = entity_window_render;
    editor_register_window(e, editor, renderer, state);
}

void entity_window_render(Vultr::Engine *e, Editor *editor, const Vultr::UpdateTick &tick, void *state)
{
    ImGui::Begin("Hierarchy");
    if (ImGui::Button("Undo"))
    {
        undo(e, editor);
    }
    if (ImGui::Button("Add Entity"))
    {
        Entity ent = create_entity(get_current_world(e));
        entity_add_component(e, ent, TransformComponent::Create(Vec3(0, 0, 0), Quat(1, 0, 0, 0), Vec3(1, 1, 1)));
    }
    Signature empty;
    for (Entity entity = 0; entity < MAX_ENTITIES; entity++)
    {
        if (get_entity_signature(get_current_world(e), entity) != empty)
        {
            if (ImGui::Selectable(("Entity " + std::to_string(entity)).c_str(), editor->selected_entity == entity))
            {
                editor->selected_entity = Entity(entity);
                if (ImGui::BeginPopupContextWindow())
                {
                    if (ImGui::MenuItem("New camera"))
                    {
                    }

                    ImGui::EndPopup();
                }
            }
        }
    }

    if (ImGui::Button("Load Scene"))
    {
        World *world = load_world(e, VultrSource("test_world.vultr"), e->component_registry);
        change_world(e, world);
        // Engine::RegisterComponents();
        // Engine::InitSystems();
        // World::FixSystems();
    }
    ImGui::End();
}
