#include <core/systems/input_system.h>
#include <core/system_providers/input_system_provider.h>
#include <core/models/update_tick.h>
#include <core/systems/gui_system.h>
#include <core/system_providers/render_system_provider.h>
#include <engine.hpp>

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
        bool prev_mouse_down = provider.mouse_down;
        provider.mouse_down = action == GLFW_PRESS;
        if (prev_mouse_down && !provider.mouse_down)
        {
            for (auto [name, event] : provider.mouse_click_events)
            {
                event(button);
            }
        }
        Input::MouseButtonInputEvent event = Input::MouseButtonInputEvent(provider.mouse_pos, provider.mouse_down);
        if (!GUISystem::receive_mouse_button_event(event))
        {
        }
    }

    static void on_scroll(GLFWwindow *window, double xamount, double yamount)
    {
        auto &provider = get_provider();
        Input::ScrollInputEvent event = Input::ScrollInputEvent(provider.mouse_pos, glm::vec2(xamount, yamount));
        add_scroll_input(glm::vec2(xamount, yamount));
        if (!GUISystem::receive_scroll_event(event))
        {
            std::cout << "GUI did not receive scroll event" << std::endl;
        }
    }

    static void on_key_input(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        auto &provider = get_provider();
        for (auto [name, event] : provider.key_press_events)
        {
            event(key, scancode, action, mods);
        }
    }

    void init(GLFWwindow *window)
    {
        auto &provider = get_provider();
        provider.window = window;
        glfwSetMouseButtonCallback(provider.window, on_mouse_input);
        glfwSetScrollCallback(provider.window, on_scroll);
        glfwSetKeyCallback(provider.window, on_key_input);
    }

    void update(const UpdateTick &tick)
    {
        auto &provider = get_provider();
        double posx, posy;
        if (!tick.debug)
        {
            glfwGetCursorPos(provider.window, &posx, &posy);
            const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
            posx /= mode->width;
            posy /= mode->height;
            posy = 1 - posy;
        }
        else
        {
            ImVec2 mouse_pos = ImGui::GetMousePos();
            posx = mouse_pos.x - RenderSystem::get_provider().game.position.x;
            posy = mouse_pos.y - RenderSystem::get_provider().game.position.y;
            posx /= RenderSystem::get_dimensions(GAME).x;
            posy /= RenderSystem::get_dimensions(GAME).y;
            posy = 1 - posy;
            if (posx > 1 || posy > 1 || posx < 0 || posy < 0)
            {
                ImVec2 mouse_pos = ImGui::GetMousePos();
                posx = mouse_pos.x - RenderSystem::get_provider().scene.position.x;
                posy = mouse_pos.y - RenderSystem::get_provider().scene.position.y;
                posx /= RenderSystem::get_dimensions(SCENE).x;
                posy /= RenderSystem::get_dimensions(SCENE).y;
                posy = 1 - posy;
                glm::vec2 n_mouse_pos = glm::vec2(posx, posy);

                if (n_mouse_pos.x <= 1 && n_mouse_pos.y <= 1 && n_mouse_pos.x >= 0 && n_mouse_pos.y >= 0)
                {
                    provider.scene_mouse_pos = n_mouse_pos;
                }
                return;
            }
        }
        glm::vec2 n_mouse_pos = glm::vec2(posx, posy);

        if (provider.mouse_pos != n_mouse_pos)
        {
            std::cout << "Within bounds" << std::endl;
            // Send mouse event to the gui system and see if it does anything
            provider.mouse_pos = n_mouse_pos;
            provider.mouse_down = glfwGetMouseButton(provider.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
            Input::MouseInputEvent event = Input::MouseInputEvent(provider.mouse_pos, provider.mouse_down);
            if (!GUISystem::receive_mouse_event(event))
            {
            }
        }
    }
    void on_mouse_click(const char *name, MouseClickEvent event)
    {
        get_provider().mouse_click_events[name] = event;
    }

    void on_key_press(const char *name, KeyPressEvent event)
    {
        get_provider().key_press_events[name] = event;
    }

} // namespace Vultr::InputSystem
