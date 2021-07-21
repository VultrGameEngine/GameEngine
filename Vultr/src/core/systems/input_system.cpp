#include <core/systems/input_system.h>
#include <core/system_providers/input_system_provider.h>
#include <core/models/update_tick.h>
#include <core/systems/gui_system.h>
#include <core/system_providers/render_system_provider.h>
#include <engine.hpp>
#include <imgui/imgui_impl_glfw.h>

namespace Vultr::InputSystem
{
    void register_system(Engine *e)
    {
        Signature signature;
        register_global_system<Component>(e, signature);
    }

    static void on_mouse_input(GLFWwindow *window, int button, int action, int mods)
    {
        Engine *e = get_engine(window);
        auto &p = get_provider(e);
        MouseButtonEvent event = {
            .pos = p.mouse_pos,
            .button = static_cast<Input::MouseButton>(button),
            .action = static_cast<Input::Action>(action),
        };
        // GUISystem::receive_mouse_button_event(event);
        for (auto &listener : p.mouse_button_listeners)
        {
            listener(e, event);
        }
    }

    static void add_scroll_input(Engine *e, Input::ScrollDir dir)
    {
        auto &p = get_provider(e);
        p.internal_scroll_queue.push(dir);
        ScrollEvent event = {
            .pos = p.mouse_pos,
            .scroll_dir = dir,
        };

        // GUISystem::receive_scroll_event(event);
        for (auto &listener : p.scroll_listeners)
        {
            listener(e, event);
        }
    }

    static void on_scroll(GLFWwindow *window, double xamount, double yamount)
    {
        Engine *e = get_engine(window);
        add_scroll_input(e, Vec2(xamount, yamount));
        ImGui_ImplGlfw_ScrollCallback(window, xamount, yamount);
    }

    static void on_key_input(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        Engine *e = get_engine(window);
        auto &p = get_provider(e);
        KeyEvent event = {
            .key = static_cast<Input::Key>(key),
            .action = static_cast<Input::Action>(action),
        };
        for (auto &listener : p.key_listeners)
        {
            listener(e, event);
        }
    } // namespace Vultr::InputSystem

    static void on_character_input(GLFWwindow *window, u32 key)
    {
        Engine *e = get_engine(window);
        auto &p = get_provider(e);
        for (auto &listener : p.character_listeners)
        {
            listener(e, key);
        }
    }

    void init(Engine *e)
    {
        auto &provider = get_provider(e);
        auto window = e->window;
        glfwSetMouseButtonCallback(window, on_mouse_input);
        glfwSetScrollCallback(window, on_scroll);
        glfwSetKeyCallback(window, on_key_input);
        glfwSetCharCallback(window, on_character_input);
    }

    void update(Engine *e, const UpdateTick &tick)
    {
        auto &p = get_provider(e);
        auto *window = e->window;

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
            g_mouse_pos.x -= RenderSystem::get_provider(e).game.position.x;
            g_mouse_pos.y -= RenderSystem::get_provider(e).game.position.y;
            g_mouse_pos.x /= RenderSystem::get_dimensions(e, GAME).x;
            g_mouse_pos.y /= RenderSystem::get_dimensions(e, GAME).y;
            g_mouse_pos.y = 1 - g_mouse_pos.y;
            p.mouse_pos = Vec2(g_mouse_pos.x, g_mouse_pos.y);

            // Scene mouse position
            ImVec2 s_mouse_pos = ImGui::GetMousePos();
            s_mouse_pos.x -= RenderSystem::get_provider(e).scene.position.x;
            s_mouse_pos.y -= RenderSystem::get_provider(e).scene.position.y;
            s_mouse_pos.x /= RenderSystem::get_dimensions(e, SCENE).x;
            s_mouse_pos.y /= RenderSystem::get_dimensions(e, SCENE).y;
            s_mouse_pos.y = 1 - s_mouse_pos.y;
            p.scene_mouse_pos = Vec2(s_mouse_pos.x, s_mouse_pos.y);
        }

        if (mouse_is_on_screen(e))
        {
            MousePositionEvent event = {.pos = p.mouse_pos};
            // if (!GUISystem::receive_mouse_position_event(event))
            // {
            for (auto &listener : p.mouse_position_listeners)
            {
                listener(e, event);
            }
            // }
        }
    }

} // namespace Vultr::InputSystem
