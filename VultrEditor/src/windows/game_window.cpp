#include <windows/game_window.h>

using namespace Vultr;
void register_game_window()
{
    void *state = static_cast<void *>(nullptr);
    WindowRenderer renderer = game_window_render;
    editor_register_window(renderer, state);
}

void game_window_render(const UpdateTick &tick, void *state)
{
    auto &texture = get_framebuffer_color_texture(RenderSystem::get_provider().game.fbo, 0);
    bind_texture(texture, GL_TEXTURE0);
    ImGui::Begin("Game");
    ImVec2 viewport_panel_size = ImGui::GetContentRegionAvail();
    ImVec2 position = ImGui::GetCursorScreenPos();
    RenderSystem::update_viewport_pos(position.x, position.y, GAME);
    RenderSystem::resize(viewport_panel_size.x, viewport_panel_size.y, GAME);
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast"
    ImGui::Image((void *)texture.id, ImVec2{viewport_panel_size.x, viewport_panel_size.y}, ImVec2{0, 1}, ImVec2{1, 0});
#pragma clang diagnostic pop
    ImGui::End();
}
