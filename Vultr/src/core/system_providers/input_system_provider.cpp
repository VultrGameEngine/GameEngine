#include <core/system_providers/input_system_provider.h>
#include <core/systems/input_system.h>

namespace Vultr::InputSystem
{
    Component &get_provider()
    {
        return *get_global_system_provider<Component>();
    }

    Input::Action get_key(Input::Key key)
    {
        return static_cast<Input::Action>(get_provider().key_states[key]);
    }

    Input::Action get_mouse_button(Input::MouseButton button)
    {
        return static_cast<Input::Action>(get_provider().mouse_button_states[button]);
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
        for (auto l : p.key_listeners)
        {
            assert(l != listener && LISTENER_REGISTERED_TWICE_ERROR);
        }
        p.key_listeners.push_back(listener);
    }

    void deregister_key_listener(OnKey listener)
    {
        auto &p = get_provider();
        auto &listeners = p.key_listeners;
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
} // namespace Vultr::InputSystem
