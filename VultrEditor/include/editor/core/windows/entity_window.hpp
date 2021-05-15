#pragma once
#include <editor/imgui/window.hpp>
#include <editor/core/windows/entity_window.hpp>
#include <ecs/entity/entity_manager.hpp>
#include <editor/editor.hpp>
#include <imgui/imgui.h>
#include <core/core_components.h>
#include <ecs/world/world.hpp>
#include <vultr.hpp>

namespace Vultr::Editor
{
    class EntityWindow : public Window
    {
      public:
        void Render() override
        {
            ImGui::Begin("Hierarchy");
            if (ImGui::Button("Add Entity"))
            {
                Entity ent = create_entity(get_current_world());
                entity_add_component(ent, TransformComponent::Create(glm::vec3(0, 0, 0), glm::quat(1, 0, 0, 0), glm::vec3(1, 1, 1)));
            }
            Signature empty;
            for (Entity entity = 0; entity < MAX_ENTITIES; entity++)
            {
                if (get_entity_signature(get_current_world(), entity) != empty)
                {
                    if (ImGui::Selectable(("Entity " + std::to_string(entity)).c_str(), Editor::Get()->selected_entity == entity))
                    {
                        Editor::Get()->selected_entity = Entity(entity);
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

            // if (ImGui::Button("Save Scene"))
            // {
            //     World::ExportWorld("test_world.world", World::Get());
            // }
            // if (ImGui::Button("Load Scene"))
            // {
            //     std::shared_ptr<World> world = World::ImportWorld("test_world.world");
            //     World::ChangeWorld(world);
            //     // Engine::RegisterComponents();
            //     // Engine::InitSystems();
            //     // World::FixSystems();
            // }
            ImGui::End();
        }
    };
} // namespace Vultr::Editor
