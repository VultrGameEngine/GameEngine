#include <editor/core/windows/entity_window.hpp>
#include <ecs/entity/entity_manager.hpp>
#include <editor/editor.hpp>
#include <imgui/imgui.h>

namespace Brick3D::Editor
{
void EntityWindow::Render()
{
    ImGui::Begin("Hierarchy");
    Signature empty;
    for (EntityID entity = 0; entity < MAX_ENTITIES; entity++)
    {
        if (Entity(entity).GetSignature() != empty)
        {
            if (ImGui::Selectable(("Entity " + std::to_string(entity)).c_str(),
                                  Editor::Get()->selected_entity.id == entity))
                Editor::Get()->selected_entity = Entity(entity);
        }
    }
    ImGui::End();
}
} // namespace Brick3D::Editor
