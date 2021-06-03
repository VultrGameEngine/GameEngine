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
static void change_editing_mode()
{
    if (InputSystem::get_key(Input::KEY_Q))
    {
        Editor::Editor::Get()->current_operation = ImGuizmo::OPERATION::TRANSLATE;
    }
    else if (InputSystem::get_key(Input::KEY_W))
    {
        Editor::Editor::Get()->current_operation = ImGuizmo::OPERATION::ROTATE;
    }
    else if (InputSystem::get_key(Input::KEY_E))
    {
        Editor::Editor::Get()->current_operation = ImGuizmo::OPERATION::SCALE;
    }
}

static void on_mouse_click(MouseButtonEvent event)
{
    if (event.button != Input::MOUSE_LEFT)
        return;
    if (!InputSystem::mouse_is_on_screen(InputSystem::get_provider().scene_mouse_pos))
        return;
    Vec2 pos = InputSystem::get_provider().scene_mouse_pos * RenderSystem::get_dimensions(SCENE);
    Entity entity = RenderSystem::get_entity_at_pixel(pos.x, pos.y);
    if (entity != INVALID_ENTITY)
        Editor::Get()->selected_entity = entity;
}

static void on_key_press(KeyEvent event)
{
    bool ctrl_down = InputSystem::get_key(Input::KEY_LEFT_CONTROL) || InputSystem::get_key(Input::KEY_RIGHT_CONTROL);
    bool shift_down = InputSystem::get_key(Input::KEY_LEFT_SHIFT) || InputSystem::get_key(Input::KEY_RIGHT_SHIFT);
    if (event.key == Input::KEY_S && event.action == Input::PRESS && ctrl_down)
    {
        Editor::Save();
        std::cout << "Saved! \n";
    }
    else if (event.key == Input::KEY_D && event.action == Input::PRESS && ctrl_down)
    {
        Editor::DuplicateEntity();
        std::cout << "Duplicated! \n";
    }
    else if (event.key == Input::KEY_Z && event.action == Input::PRESS && ctrl_down)
    {
        if (shift_down)
        {
            Editor::Redo();
            std::cout << "Redo! \n";
        }
        else
        {
            Editor::Undo();
            std::cout << "Undo! \n";
        }
    }
    else if (event.key == Input::KEY_DELETE && event.action == Input::PRESS && Editor::Get()->selected_entity != INVALID_ENTITY)
    {
        Editor::DeleteEntity();
    }
}

SceneWindow::SceneWindow()
{
    InputSystem::set_mouse_button_listener(on_mouse_click);
    InputSystem::set_key_listener(on_key_press);
}

void SceneWindow::Render()
{
    // Save the system providers here for easy access
    auto &render_system_provider = RenderSystem::get_provider();
    auto &camera_system_provider = CameraSystem::get_provider();

    // For the scene window we will need the scene texture
    render_system_provider.scene.render_texture->Bind(GL_TEXTURE0);

    // Draw all imgui windows
    ImGui::Begin("Scene");
    ImVec2 viewport_panel_size = ImGui::GetContentRegionAvail();
    RenderSystem::resize(viewport_panel_size.x, viewport_panel_size.y, SCENE);
    ImVec2 position = ImGui::GetCursorScreenPos();
    RenderSystem::update_viewport_pos(position.x, position.y, SCENE);

    // Get rid of incorrect warnings
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast"

    // Draw the scene window texture
    ImGui::Image((void *)render_system_provider.scene.render_texture->GetID(), ImVec2{viewport_panel_size.x, viewport_panel_size.y}, ImVec2{0, 1}, ImVec2{1, 0});

#pragma clang diagnostic pop

    // Update the editing mode based on the user input in the editor
    change_editing_mode();

    // Get the currently selected entity in the EntityWindow
    Entity selected_entity = Editor::Get()->selected_entity;

    if (selected_entity != INVALID_ENTITY)
    {

        // ImGuizmo rendering
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();

        // Window dimensions and mouse positions
        float windowWidth = (float)ImGui::GetWindowWidth();
        float windowHeight = (float)ImGui::GetWindowHeight();
        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

        // Editor camera
        const auto &editor_camera_component = camera_system_provider.scene_camera.camera_component;
        const auto &editor_camera_transform = camera_system_provider.scene_camera.transform_component;
        const auto &cameraProjection = editor_camera_component.GetProjectionMatrix(RenderSystem::get_dimensions(SCENE).x, RenderSystem::get_dimensions(SCENE).y);
        auto cameraView = editor_camera_transform.GetViewMatrix();

        // Entity transform
        auto *tc = entity_get_component_unsafe<TransformComponent>(selected_entity);

        // TODO Fix this, there is an issue where entity_get_component will cause an assert and crash the editor, I'm not entirely sure what the issue is because it doesn't seem like anything is dependent on the
        // selected_entity of the editor thus no threading issues it seems?
        //
        // In the mean time, use entity_get_component_unsafe just to make sure that the editor doesn't crash and simply quietly handles it, will fix it if it does actually cause problems
        //
        // If an entity is selected
        if (tc != nullptr)
        {
            Mat4 transform = tc->Matrix();
            static bool was_using_guizmo = false;
            static TransformComponent copy;

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
                // If we weren't previously using this guizmo, then we want to save a copy of the transform component
                if (!was_using_guizmo)
                {
                    copy = *tc;
                }
                Vec3 translation, rotation, scale;
                Math::decompose_transform(transform, translation, rotation, scale);

                Vec3 deltaRotation = rotation - glm::eulerAngles(tc->rotation);
                tc->position = translation;
                tc->rotation = Quat(rotation);
                tc->scale = scale;
            }
            // If we just stopped using this guizmo, push this event to the edit stack
            else if (was_using_guizmo)
            {
                auto *event = new ComponentEditEvent<TransformComponent>();
                event->before = copy;
                event->after = *tc;
                event->entities = {selected_entity};
                event->type = get_component_type<TransformComponent>();
                engine_send_update_event(event);
            }

            was_using_guizmo = ImGuizmo::IsUsing();
        }
        else
        {
            std::cout << "Error getting transform component from entity" << std::endl;
        }
    }
    ImGui::End();
}
