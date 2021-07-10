#include <core/system_providers/input_system_provider.h>
#include <core/systems/input_system.h>

namespace Vultr::InputSystem
{
    Component &get_provider()
    {
        return *get_global_system_provider<Component>();
    }

    static Input::Action get_action_from_keys(Input::Key k1, Input::Key k2)
    {
        s32 k1A = glfwGetKey(engine_global()->window, k1);
        s32 k2A = glfwGetKey(engine_global()->window, k2);
        if (k1A || k2A)
        {
            if (k1A > k2A)
            {
                return static_cast<Input::Action>(k1A);
            }
            else
            {
                return static_cast<Input::Action>(k2A);
            }
        }
        else
        {
            return Input::RELEASE;
        }
    }

    Input::Action get_key(Input::Key key)
    {
        if (key == Input::KEY_SHIFT)
        {
            return get_action_from_keys(Input::KEY_LEFT_SHIFT, Input::KEY_RIGHT_SHIFT);
        }
        else if (key == Input::KEY_CONTROL)
        {
            return get_action_from_keys(Input::KEY_LEFT_CONTROL, Input::KEY_RIGHT_CONTROL);
        }
        else if (key == Input::KEY_ALT)
        {
            return get_action_from_keys(Input::KEY_LEFT_ALT, Input::KEY_RIGHT_ALT);
        }
        else if (key == Input::KEY_SUPER)
        {
            return get_action_from_keys(Input::KEY_LEFT_SUPER, Input::KEY_RIGHT_SUPER);
        }
        else
        {
            return static_cast<Input::Action>(glfwGetKey(engine_global()->window, key));
        }
    }

    Input::Action get_mouse_button(Input::MouseButton button)
    {
        return static_cast<Input::Action>(glfwGetMouseButton(engine_global()->window, button));
    }

    bool mouse_is_on_screen()
    {
        return mouse_is_on_screen(get_provider().mouse_pos);
    }

    bool mouse_is_on_screen(Vec2 pos)
    {
        return pos.x <= 1 && pos.x >= 0 && pos.y <= 1 && pos.y >= 0;
    }

    const Input::MousePos &get_mouse_position()
    {
        return get_provider().mouse_pos;
    }

    const Input::MousePos &get_scene_mouse_position()
    {
        return get_provider().scene_mouse_pos;
    }

#define LISTENER_REGISTERED_TWICE_ERROR "Listener already registered! Deregister your mouse position listener by calling "
#define LISTENER_NOT_FOUND_ERROR "Failed to deregister, listener not found!"

    void set_mouse_pos_listener(OnMousePositionChange listener)
    {
        auto &p = get_provider();
        for (auto l : p.mouse_position_listeners)
        {
            assert(l != listener && LISTENER_REGISTERED_TWICE_ERROR);
        }
        p.mouse_position_listeners.push_back(listener);
    }

    void deregister_mouse_pos_listener(OnMousePositionChange listener)
    {
        auto &p = get_provider();
        auto &listeners = p.mouse_position_listeners;
        u32 index = 0;
        for (auto l : listeners)
        {
            if (l == listener)
            {
                listeners.erase(listeners.begin() + index);
            }
            index++;
        }
        assert(true && LISTENER_NOT_FOUND_ERROR);
    }

    void set_mouse_button_listener(OnMouseButton listener)
    {
        auto &p = get_provider();
        for (auto l : p.mouse_button_listeners)
        {
            assert(l != listener && LISTENER_REGISTERED_TWICE_ERROR);
        }
        p.mouse_button_listeners.push_back(listener);
    }

    void deregister_mouse_button_listener(OnMouseButton listener)
    {
        auto &p = get_provider();
        auto &listeners = p.mouse_button_listeners;
        u32 index = 0;
        for (auto l : listeners)
        {
            if (l == listener)
            {
                listeners.erase(listeners.begin() + index);
            }
            index++;
        }
        assert(true && LISTENER_NOT_FOUND_ERROR);
    }

    void set_scroll_listener(OnScroll listener)
    {
        auto &p = get_provider();
        for (auto l : p.scroll_listeners)
        {
            assert(l != listener && LISTENER_REGISTERED_TWICE_ERROR);
        }
        p.scroll_listeners.push_back(listener);
    }

    void deregister_scroll_listener(OnScroll listener)
    {
        auto &p = get_provider();
        auto &listeners = p.scroll_listeners;
        u32 index = 0;
        for (auto l : listeners)
        {
            if (l == listener)
            {
                listeners.erase(listeners.begin() + index);
            }
            index++;
        }
        assert(true && LISTENER_NOT_FOUND_ERROR);
    }

    void set_key_listener(OnKey listener)
    {
        auto &p = get_provider();
        // for (auto l : p.key_listeners)
        // {
        //     assert(l != listener && LISTENER_REGISTERED_TWICE_ERROR);
        // }
        p.key_listeners.push_back(listener);
    }

    void deregister_key_listener(OnKey listener)
    {
        // auto &p = get_provider();
        // auto &listeners = p.key_listeners;
        // u32 index = 0;
        // for (auto l : listeners)
        // {
        //     if (l == listener)
        //     {
        //         listeners.erase(listeners.begin() + index);
        //     }
        //     index++;
        // }
        assert(true && LISTENER_NOT_FOUND_ERROR);
    }

    void set_character_listener(OnCharacter listener)
    {
        auto &p = get_provider();
        p.character_listeners.push_back(listener);
    }
    void deregister_character_listener(OnCharacter listener)
    {
    }
} // namespace Vultr::InputSystem
