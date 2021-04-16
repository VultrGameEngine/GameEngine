#include <core/system_providers/render_system_provider.h>
#include <editor/core/windows/game_window.hpp>

#include <imgui/imgui.h>

#include <glad/glad.h>

namespace Vultr
{
void Editor::GameWindow::Render()
{
    RenderSystemProvider::Get()->game.render_texture->Bind(GL_TEXTURE0);
    ImGui::Begin("Game");
    ImVec2 viewport_panel_size = ImGui::GetContentRegionAvail();
    ImVec2 position = ImGui::GetCursorScreenPos();
    RenderSystemProvider::UpdateViewportPos(position.x, position.y, GAME);
    RenderSystemProvider::Resize(viewport_panel_size.x, viewport_panel_size.y, GAME);
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast"
    ImGui::Image((void *)RenderSystemProvider::Get()->game.render_texture->GetID(),
                 ImVec2{viewport_panel_size.x, viewport_panel_size.y}, ImVec2{0, 1},
                 ImVec2{1, 0});
#pragma clang diagnostic pop
    ImGui::End();
}
} // namespace Vultr
