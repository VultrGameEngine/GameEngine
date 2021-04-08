#include <core/system_providers/render_system_provider.h>
#include <core/system_providers/camera_system_provider.h>
#include <math/decompose_transform.h>
#include <editor/core/windows/scene_window.hpp>
#include <editor/editor.hpp>
#include <ImGuizmo/ImGuizmo.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
    if (Vultr::Editor::Editor::Get()->selected_entity.id != -1)
    {
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();

        float windowWidth = (float)ImGui::GetWindowWidth();
        float windowHeight = (float)ImGui::GetWindowHeight();
        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y,
                          windowWidth, windowHeight);

        // Editor camera
        const auto &editor_camera_component =
            Vultr::CameraSystemProvider::Get()->m_scene_camera.camera_component;
        const auto &editor_camera_transform =
            Vultr::CameraSystemProvider::Get()->m_scene_camera.transform_component;
        const glm::mat4 &cameraProjection =
            editor_camera_component.GetProjectionMatrix(
                Vultr::RenderSystemProvider::GetDimensions(SCENE).x,
                Vultr::RenderSystemProvider::GetDimensions(SCENE).y);
        glm::mat4 cameraView = editor_camera_transform.GetViewMatrix();

        // Entity transform
        auto &tc = Vultr::Editor::Editor::Get()
                       ->selected_entity.GetComponent<TransformComponent>();
        glm::mat4 transform = tc.Matrix();

        // Snapping
        // bool snap = Input::IsKeyPressed(Key::LeftControl);
        // float snapValue = 0.5f; // Snap to 0.5m for translation/scale
        // // Snap to 45 degrees for rotation
        // if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
        //     snapValue = 45.0f;

        // float snapValues[3] = {snapValue, snapValue, snapValue};

        ImGuizmo::Manipulate(
            glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
            (ImGuizmo::OPERATION)ImGuizmo::OPERATION::ROTATE, ImGuizmo::LOCAL,
            glm::value_ptr(transform), nullptr, nullptr);

        if (ImGuizmo::IsUsing())
        {
            glm::vec3 translation, rotation, scale;
            Math::DecomposeTransform(transform, translation, rotation, scale);

            glm::vec3 deltaRotation = rotation - glm::eulerAngles(tc.rotation);
            tc.position = translation;
            tc.rotation = glm::quat(rotation);
            tc.scale = scale;
        }
        ImGui::End();
    }
}
} // namespace Vultr::Editor
