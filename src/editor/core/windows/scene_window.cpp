#include <core/system_providers/render_system_provider.h>
#include <editor/core/windows/scene_window.hpp>

#include <imgui/imgui.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Brick3D::Editor
{
void SceneWindow::Render()
{
    glActiveTexture(GL_TEXTURE0);
    RenderSystemProvider::Get().scene.render_texture->Bind();
    ImGui::Begin("Scene");
    ImVec2 viewport_panel_size = ImGui::GetContentRegionAvail();
    RenderSystemProvider::Resize(viewport_panel_size.x, viewport_panel_size.y,
                                 SCENE);
    ImGui::Image((void *)RenderSystemProvider::Get().scene.render_texture,
                 ImVec2{viewport_panel_size.x, viewport_panel_size.y}, ImVec2{0, 1},
                 ImVec2{1, 0});
    ImGui::End();
}
} // namespace Brick3D::Editor
