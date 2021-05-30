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
        auto &provider = get_provider();
        MouseButtonEvent event = {
            .pos = provider.mouse_pos,
            .button = static_cast<Input::MouseButton>(button),
            .action = static_cast<Input::Action>(action),
        };

        if (!GUISystem::receive_mouse_button_event(event))
        {
            for (auto listener : provider.mouse_button_listeners)
            {
                listener(event);
            }
        }
    }

    static void add_scroll_input(Input::ScrollDir dir)
    {
        auto &p = get_provider();
        p.internal_scroll_queue.push(dir);
    }

    static void on_scroll(GLFWwindow *window, double xamount, double yamount)
    {
        auto &provider = get_provider();
        add_scroll_input(Vec2(xamount, yamount));
        ScrollEvent event = {
            .pos = provider.mouse_pos,
            .scroll_dir = Input::ScrollDir(xamount, yamount),
        };
        if (!GUISystem::receive_scroll_event(event))
        {
            std::cout << "GUI did not receive scroll event" << std::endl;
        }
        ImGui_ImplGlfw_ScrollCallback(window, xamount, yamount);
    }

    static void on_key_input(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        auto &provider = get_provider();
        // for (auto [name, event] : provider.key_press_events)
        // {
        //     event(key, scancode, action, mods);
        // }
    }

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
        auto &provider = get_provider();
        // double posx, posy;
        // if (!tick.debug)
        // {
        //     glfwGetCursorPos(provider.window, &posx, &posy);
        //     const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        //     posx /= mode->width;
        //     posy /= mode->height;
        //     posy = 1 - posy;
        // }
        // else
        // {
        //     ImVec2 mouse_pos = ImGui::GetMousePos();
        //     posx = mouse_pos.x - RenderSystem::get_provider().game.position.x;
        //     posy = mouse_pos.y - RenderSystem::get_provider().game.position.y;
        //     posx /= RenderSystem::get_dimensions(GAME).x;
        //     posy /= RenderSystem::get_dimensions(GAME).y;
        //     posy = 1 - posy;
        //     if (posx > 1 || posy > 1 || posx < 0 || posy < 0)
        //     {
        //         ImVec2 mouse_pos = ImGui::GetMousePos();
        //         posx = mouse_pos.x - RenderSystem::get_provider().scene.position.x;
        //         posy = mouse_pos.y - RenderSystem::get_provider().scene.position.y;
        //         posx /= RenderSystem::get_dimensions(SCENE).x;
        //         posy /= RenderSystem::get_dimensions(SCENE).y;
        //         posy = 1 - posy;
        //         Vec2 n_mouse_pos = Vec2(posx, posy);

        //         if (n_mouse_pos.x <= 1 && n_mouse_pos.y <= 1 && n_mouse_pos.x >= 0 && n_mouse_pos.y >= 0)
        //         {
        //             provider.scene_mouse_pos = n_mouse_pos;
        //         }
        //         return;
        //     }
        // }
        // Vec2 n_mouse_pos = Vec2(posx, posy);

        // if (provider.mouse_pos != n_mouse_pos)
        // {
        //     std::cout << "Within bounds" << std::endl;
        //     // Send mouse event to the gui system and see if it does anything
        //     provider.mouse_pos = n_mouse_pos;
        //     provider.mouse_down = glfwGetMouseButton(provider.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
        //     Input::MouseInputEvent event = Input::MouseInputEvent(provider.mouse_pos, provider.mouse_down);
        //     if (!GUISystem::receive_mouse_event(event))
        //     {
        //     }
        // }
    }

} // namespace Vultr::InputSystem
