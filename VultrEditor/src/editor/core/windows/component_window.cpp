#include <editor/core/windows/component_window.hpp>
#include <editor/editor.hpp>
#include <core/component_renderer.h>
#include <ecs/world/world.hpp>

namespace Vultr::Editor
{
void ComponentWindow::Render()
{
    ImGui::Begin("Inspector");
    Entity entity = Editor::Get()->selected_entity;
    if (entity.id >= 0)
    {
        World::Get()->component_manager->RenderEntityComponents(
            Editor::Get()->selected_entity);
    }

    ImGui::End();
}
} // namespace Vultr::Editor
