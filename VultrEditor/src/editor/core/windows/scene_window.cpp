#include <core/system_providers/render_system_provider.h>
#include <core/systems/render_system.h>
#include <core/system_providers/camera_system_provider.h>
#include <core/system_providers/input_system_provider.h>
#include <math/decompose_transform.h>
#include <editor/core/windows/scene_window.hpp>
#include <editor/editor.hpp>
#include <ImGuizmo/ImGuizmo.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <engine.hpp>

#include <imgui/imgui.h>

#include <glad/glad.h>

using namespace Vultr;
void SceneWindow::Render()
{
    auto &render_system_provider = RenderSystem::get_provider();
    if (InputSystem::key_down('q'))
    {
        Editor::Editor::Get()->current_operation = ImGuizmo::OPERATION::TRANSLATE;
    }
    else if (InputSystem::key_down('w'))
    {
        Editor::Editor::Get()->current_operation = ImGuizmo::OPERATION::ROTATE;
    }
    else if (InputSystem::key_down('e'))
    {
        Editor::Editor::Get()->current_operation = ImGuizmo::OPERATION::SCALE;
    }
    render_system_provider.scene.render_texture->Bind(GL_TEXTURE0);
    ImGui::Begin("Scene");
    ImVec2 viewport_panel_size = ImGui::GetContentRegionAvail();
    RenderSystem::resize(viewport_panel_size.x, viewport_panel_size.y, SCENE);
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast"
    ImGui::Image((void *)render_system_provider.scene.render_texture->GetID(), ImVec2{viewport_panel_size.x, viewport_panel_size.y}, ImVec2{0, 1}, ImVec2{1, 0});
#pragma clang diagnostic pop

    // pos *= RenderSystemProvider::Get()->GetDimensions(SCENE);
    // int pickedID = RenderSystemProvider::Get()->GetEntityAtPixel(pos.x, pos.y);
    // std::cout << "Pixel data at " << pos.x << ", " << pos.y << ": " << pickedID
    //           << std::endl;
    Entity selected_entity = Editor::Get()->selected_entity;
    if (selected_entity != INVALID_ENTITY)
    {
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();

        float windowWidth = (float)ImGui::GetWindowWidth();
        float windowHeight = (float)ImGui::GetWindowHeight();
        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

        auto &camera_system_provider = CameraSystem::get_provider();

        // Editor camera
        const auto &editor_camera_component = camera_system_provider.scene_camera.camera_component;
        const auto &editor_camera_transform = camera_system_provider.scene_camera.transform_component;
        const glm::mat4 &cameraProjection = editor_camera_component.GetProjectionMatrix(RenderSystem::get_dimensions(SCENE).x, RenderSystem::get_dimensions(SCENE).y);
        glm::mat4 cameraView = editor_camera_transform.GetViewMatrix();

        // Entity transform
        auto &tc = entity_get_component<TransformComponent>(selected_entity);
        glm::mat4 transform = tc.Matrix();

        // Snapping
        // bool snap = Input::IsKeyPressed(Key::LeftControl);
        // float snapValue = 0.5f; // Snap to 0.5m for translation/scale
        // // Snap to 45 degrees for rotation
        // if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
        //     snapValue = 45.0f;

        // float snapValues[3] = {snapValue, snapValue, snapValue};

        ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), (ImGuizmo::OPERATION)Editor::Get()->current_operation, ImGuizmo::LOCAL, glm::value_ptr(transform), nullptr, nullptr);

        if (ImGuizmo::IsUsing())
        {
            glm::vec3 translation, rotation, scale;
            Math::DecomposeTransform(transform, translation, rotation, scale);

            glm::vec3 deltaRotation = rotation - glm::eulerAngles(tc.rotation);
            tc.position = translation;
            tc.rotation = glm::quat(rotation);
            tc.scale = scale;
        }
    }
    ImGui::End();
}
