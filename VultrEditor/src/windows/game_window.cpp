#include <windows/game_window.h>

using namespace Vultr;
void register_game_window(Vultr::Engine *e, Editor *editor)
{
    void *state = static_cast<void *>(nullptr);
    WindowRenderer renderer = game_window_render;
    editor_register_window(e, editor, renderer, state);
}

void game_window_render(Vultr::Engine *e, Editor *editor, const UpdateTick &tick, void *state)
{
    auto *texture = get_framebuffer_color_texture(&RenderSystem::get_provider(e).game.post_processed_fbo, 0);
    bind_texture(texture, GL_TEXTURE0);
    ImGui::Begin("Game");
    ImVec2 viewport_panel_size = ImGui::GetContentRegionAvail();
    ImVec2 position = ImGui::GetCursorScreenPos();
    RenderSystem::update_viewport_pos(e, position.x, position.y, GAME);
    RenderSystem::resize(e, viewport_panel_size.x, viewport_panel_size.y, GAME);
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast"
    ImGui::Image((void *)texture->id, ImVec2{viewport_panel_size.x, viewport_panel_size.y}, ImVec2{0, 1}, ImVec2{1, 0});
#pragma clang diagnostic pop
    ImGui::End();
}
