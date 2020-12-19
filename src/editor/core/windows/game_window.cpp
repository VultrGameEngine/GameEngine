#include "../../../../include/editor/core/windows/game_window.hpp"
#include "../../../../include/core/systems/render_system.h"

#include "../../../../vendor/imgui/imgui.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

void Editor::GameWindow::Render()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, RenderSystem::Get()->game.render_texture);
    ImGui::Begin("Game");
    ImVec2 viewport_panel_size = ImGui::GetContentRegionAvail();
    RenderSystem::Resize(viewport_panel_size.x, viewport_panel_size.y, GAME);
    ImGui::Image((void *)RenderSystem::Get()->game.render_texture, ImVec2{viewport_panel_size.x, viewport_panel_size.y}, ImVec2{0, 1}, ImVec2{1, 0});
    ImGui::End();
}