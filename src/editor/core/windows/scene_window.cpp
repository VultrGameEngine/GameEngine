#include "../../../../include/editor/core/windows/scene_window.hpp"
#include "../../../../include/core/systems/render_system.h"

#include "../../../../vendor/imgui/imgui.h"

void Editor::SceneWindow::Render()
{
    ImGui::Begin("Scene");
    ImVec2 viewport_panel_size = ImGui::GetContentRegionAvail();
    RenderSystem::Resize(viewport_panel_size.x, viewport_panel_size.y);
    ImGui::Image((void *)RenderSystem::Get()->render_texture, ImVec2{viewport_panel_size.x, viewport_panel_size.y}, ImVec2{0, 1}, ImVec2{1, 0});
    ImGui::End();
}