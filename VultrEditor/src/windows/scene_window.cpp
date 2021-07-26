#include <windows/scene_window.h>
#include <editor.h>
#include <core/system_providers/render_system_provider.h>
#include <core/systems/render_system.h>
#include <core/system_providers/camera_system_provider.h>
#include <core/system_providers/input_system_provider.h>
#include <math/decompose_transform.h>
#include <ImGuizmo/ImGuizmo.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <core/models/event.h>

using namespace Vultr;

static void on_mouse_click(Engine *e, Editor *editor, MouseButtonEvent event)
{
    if (event.button != Input::MOUSE_LEFT)
        return;
    if (!InputSystem::mouse_is_on_screen(e, InputSystem::get_provider(e).scene_mouse_pos))
        return;
    Vec2 pos = InputSystem::get_provider(e).scene_mouse_pos * RenderSystem::get_dimensions(e, SCENE);
    Entity entity = RenderSystem::get_entity_at_pixel(e, pos.x, pos.y);
    if (entity != INVALID_ENTITY)
        editor->selected_entity = entity;
}

// TODO: AHHH GLOBAL FUCKING VARIABLE WHAT IS WRONG WITH ME
static SceneWindow *&get_scene_window()
{
    static SceneWindow *window = nullptr;
    return window;
}

static void on_key_press(Engine *e, Editor *editor, KeyEvent event)
{
    if (event.action != Input::PRESS)
        return;
    switch (event.key)
    {
        case Input::KEY_S:
        {
            if (InputSystem::get_key(e, Input::KEY_CONTROL))
            {
                save(e, editor);
                std::cout << "Saved! \n";
            }
            break;
        }
        case Input::KEY_D:
        {
            if (InputSystem::get_key(e, Input::KEY_CONTROL))
            {
                duplicate_entity(e, editor);
                std::cout << "Duplicated! \n";
            }
            break;
        }
        case Input::KEY_Z:
        {
            if (InputSystem::get_key(e, Input::KEY_CONTROL))
            {
                if (InputSystem::get_key(e, Input::KEY_SHIFT))
                {
                    redo(e, editor);
                    std::cout << "Redo! \n";
                }
                else
                {
                    undo(e, editor);
                    std::cout << "Undo! \n";
                }
            }
            break;
        }
        case Input::KEY_DELETE:
        {
            if (editor->selected_entity != INVALID_ENTITY)
            {
                delete_entity(e, editor);
            }
            break;
        }
        case Input::KEY_Q:
        {
            if (InputSystem::get_mouse_button(e, Input::MOUSE_RIGHT) == 0)
            {
                get_scene_window()->current_operation = ImGuizmo::OPERATION::TRANSLATE;
            }
            break;
        }
        case Input::KEY_W:
        {
            if (InputSystem::get_mouse_button(e, Input::MOUSE_RIGHT) == 0)
            {
                get_scene_window()->current_operation = ImGuizmo::OPERATION::ROTATE;
            }
            break;
        }
        case Input::KEY_E:
        {
            if (InputSystem::get_mouse_button(e, Input::MOUSE_RIGHT) == 0)
            {
                get_scene_window()->current_operation = ImGuizmo::OPERATION::SCALE;
            }
            break;
        }
        default:
            break;
    }
}

void register_scene_window(Engine *e, Editor *editor)
{
    void *state = static_cast<void *>(new SceneWindow());
    get_scene_window() = static_cast<SceneWindow *>(state);
    editor_register_window(e, editor, scene_window_render, state);
    InputSystem::set_mouse_button_listener(e, [editor](Vultr::Engine *e, MouseButtonEvent event) { on_mouse_click(e, editor, event); });
    InputSystem::set_key_listener(e, [editor](Vultr::Engine *e, KeyEvent event) { on_key_press(e, editor, event); });
}

void scene_window_render(Engine *e, Editor *editor, const UpdateTick &tick, void *_state)
{
    // General state things
    auto *state = static_cast<SceneWindow *>(_state);

    // Save the system providers here for easy access
    auto &render_system_provider = RenderSystem::get_provider(e);
    auto &camera_system_provider = CameraSystem::get_provider(e);

    // For the scene window we will need the scene texture
    auto texture = get_framebuffer_color_texture(render_system_provider.scene.fbo, 0);
    bind_texture(texture, GL_TEXTURE0);

    // Draw all imgui windows
    ImGui::Begin("Scene");
    ImVec2 viewport_panel_size = ImGui::GetContentRegionAvail();
    RenderSystem::resize(e, viewport_panel_size.x, viewport_panel_size.y, SCENE);
    ImVec2 position = ImGui::GetCursorScreenPos();
    RenderSystem::update_viewport_pos(e, position.x, position.y, SCENE);

    // Get rid of incorrect warnings
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast"

    // Draw the scene window texture
    ImGui::Image((void *)texture.id, ImVec2{viewport_panel_size.x, viewport_panel_size.y}, ImVec2{0, 1}, ImVec2{1, 0});

#pragma clang diagnostic pop

    // Get the currently selected entity in the EntityWindow
    Entity selected_entity = editor->selected_entity;

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
        const auto &cameraProjection = editor_camera_component.GetProjectionMatrix(RenderSystem::get_dimensions(e, SCENE).x, RenderSystem::get_dimensions(e, SCENE).y);
        auto cameraView = editor_camera_transform.GetViewMatrix();

        // Entity transform
        auto *tc = entity_get_component_unsafe<TransformComponent>(e, selected_entity);

        // If an entity is selected
        if (tc != nullptr)
        {
            Mat4 transform = tc->Matrix();
            static bool was_using_guizmo = false;
            static TransformComponent copy;

            bool snap = InputSystem::get_key(e, Input::KEY_SHIFT) > 0;
            float snap_value = 0.5f;

            if (state->current_operation == ImGuizmo::OPERATION::ROTATE)
                snap_value = 45.0f;

            float snap_values[3] = {snap_value, snap_value, snap_value};

            ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), (ImGuizmo::OPERATION)state->current_operation, ImGuizmo::LOCAL, glm::value_ptr(transform), nullptr,
                                 snap ? snap_values : nullptr);

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
                event->type = get_component_type<TransformComponent>(e);
                engine_send_update_event(e, event);
            }

            was_using_guizmo = ImGuizmo::IsUsing();
        }
    }
    ImGui::End();
}
