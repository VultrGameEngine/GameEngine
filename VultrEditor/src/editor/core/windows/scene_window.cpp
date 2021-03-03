#include <core/system_providers/render_system_provider.h>
#include <editor/core/windows/scene_window.hpp>

#include <imgui/imgui.h>

#include <glad/glad.h>

namespace Vultr::Editor
{
void SceneWindow::Render()
{
    RenderSystemProvider::Get()->scene.render_texture->Bind(GL_TEXTURE0);
    ImGui::Begin("Scene");
    ImVec2 viewport_panel_size = ImGui::GetContentRegionAvail();
    RenderSystemProvider::Resize(viewport_panel_size.x, viewport_panel_size.y,
                                 SCENE);
    ImGui::Image((void *)RenderSystemProvider::Get()->scene.render_texture->GetID(),
                 ImVec2{viewport_panel_size.x, viewport_panel_size.y}, ImVec2{0, 1},
                 ImVec2{1, 0});
    ImGui::End();
}
} // namespace Vultr::Editor
