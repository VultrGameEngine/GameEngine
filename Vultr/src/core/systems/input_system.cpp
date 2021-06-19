#include <core/systems/input_system.h>
#include <core/system_providers/input_system_provider.h>
#include <core/models/update_tick.h>
#include <core/systems/gui_system.h>
#include <core/system_providers/render_system_provider.h>
#include <engine.hpp>
#include <imgui/imgui_impl_glfw.h>

namespace Vultr::InputSystem
{
    void register_system()
    {
        Signature signature;
        register_global_system<Component>(signature);
    }

    static void on_mouse_input(GLFWwindow *window, int button, int action, int mods)
    {
        auto &p = get_provider();
        MouseButtonEvent event = {
            .pos = p.mouse_pos,
            .button = static_cast<Input::MouseButton>(button),
            .action = static_cast<Input::Action>(action),
        };
        // GUISystem::receive_mouse_button_event(event);
        for (auto &listener : p.mouse_button_listeners)
        {
            listener(event);
        }
    }

    static void add_scroll_input(Input::ScrollDir dir)
    {
        auto &p = get_provider();
        p.internal_scroll_queue.push(dir);
        ScrollEvent event = {
            .pos = p.mouse_pos,
            .scroll_dir = dir,
        };

        // GUISystem::receive_scroll_event(event);
        for (auto &listener : p.scroll_listeners)
        {
            listener(event);
        }
    }

    static void on_scroll(GLFWwindow *window, double xamount, double yamount)
    {
        add_scroll_input(Vec2(xamount, yamount));
        ImGui_ImplGlfw_ScrollCallback(window, xamount, yamount);
    }

    static void on_key_input(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        auto &p = get_provider();
        KeyEvent event = {
            .key = static_cast<Input::Key>(key),
            .action = static_cast<Input::Action>(action),
        };
        for (auto &listener : p.key_listeners)
        {
            listener(event);
        }
    } // namespace Vultr::InputSystem

    void init()
    {
        auto &provider = get_provider();
        auto *window = engine_global()->window;
        glfwSetMouseButtonCallback(window, on_mouse_input);
        glfwSetScrollCallback(window, on_scroll);
        glfwSetKeyCallback(window, on_key_input);
    }

    void update(const UpdateTick &tick)
    {
        auto &p = get_provider();
        auto *window = engine_global()->window;

        // These are x and y doubles that will be used to retreive the current mouse pos
        // they will later be put into a Vec2. They have to be separate vars because
        // glfwGetCursorPos requires doubles not floats
        f64 x, y;

        if (!tick.debug)
        {
            const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
            glfwGetCursorPos(window, &x, &y);
            x /= mode->width;
            y /= mode->height;
            y = 1 - y;

            Vec2 mouse_pos = Vec2(x, y);
            p.mouse_pos = mouse_pos;
        }
        else
        {
            // Game mouse position
            ImVec2 g_mouse_pos = ImGui::GetMousePos();
            g_mouse_pos.x -= RenderSystem::get_provider().game.position.x;
            g_mouse_pos.y -= RenderSystem::get_provider().game.position.y;
            g_mouse_pos.x /= RenderSystem::get_dimensions(GAME).x;
            g_mouse_pos.y /= RenderSystem::get_dimensions(GAME).y;
            g_mouse_pos.y = 1 - g_mouse_pos.y;
            p.mouse_pos = Vec2(g_mouse_pos.x, g_mouse_pos.y);

            // Scene mouse position
            ImVec2 s_mouse_pos = ImGui::GetMousePos();
            s_mouse_pos.x -= RenderSystem::get_provider().scene.position.x;
            s_mouse_pos.y -= RenderSystem::get_provider().scene.position.y;
            s_mouse_pos.x /= RenderSystem::get_dimensions(SCENE).x;
            s_mouse_pos.y /= RenderSystem::get_dimensions(SCENE).y;
            s_mouse_pos.y = 1 - s_mouse_pos.y;
            p.scene_mouse_pos = Vec2(s_mouse_pos.x, s_mouse_pos.y);
        }

        if (mouse_is_on_screen())
        {
            MousePositionEvent event = {.pos = p.mouse_pos};
            // if (!GUISystem::receive_mouse_position_event(event))
            // {
            for (auto &listener : p.mouse_position_listeners)
            {
                listener(event);
            }
            // }
        }
    }

} // namespace Vultr::InputSystem
