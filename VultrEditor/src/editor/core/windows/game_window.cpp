#include <core/system_providers/render_system_provider.h>
#include <editor/core/windows/game_window.hpp>

#include <imgui/imgui.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Brick3D
{
void Editor::GameWindow::Render()
{
    RenderSystemProvider::Get().game.render_texture->Bind(GL_TEXTURE0);
    ImGui::Begin("Game");
    ImVec2 viewport_panel_size = ImGui::GetContentRegionAvail();
    RenderSystemProvider::Resize(viewport_panel_size.x, viewport_panel_size.y, GAME);
    ImGui::Image((void *)RenderSystemProvider::Get().game.render_texture->GetID(),
                 ImVec2{viewport_panel_size.x, viewport_panel_size.y}, ImVec2{0, 1},
                 ImVec2{1, 0});
    ImGui::End();
}
} // namespace Brick3D
