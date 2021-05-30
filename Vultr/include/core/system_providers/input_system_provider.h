#pragma once
#include <ecs/system/system_provider.hpp>
#include <engine.hpp>
#include <GLFW/glfw3.h>
#include <queue>
#include <input/input.h>

namespace Vultr
{
    namespace InputSystem
    {
        struct Component : public SystemProvider
        {
            // Array holding the current states of different input
            // the index is the button and the value is the action, either released or pressed
            s16 key_states[348];
            s16 mouse_button_states[8];

            Input::MousePos mouse_pos = Input::MousePos(0, 0);
            Input::MousePos scene_mouse_pos = Input::MousePos(0, 0);

            // Internal
            std::queue<Vec2> internal_scroll_queue;

            // Event queue, to make sure things are synchronous and we don't have thread issues with glfw calling our events randomly
            std::queue<MousePositionEvent> internal_mouse_position_event_queue;
            std::queue<MouseButtonEvent> internal_mouse_button_event_queue;
            std::queue<ScrollEvent> internal_scroll_event_queue;
            std::queue<KeyEvent> internal_key_event_queue;

            std::vector<OnMousePositionChange> mouse_position_listeners;
            std::vector<OnMouseButton> mouse_button_listeners;
            std::vector<OnScroll> scroll_listeners;
            std::vector<OnKey> key_listeners;
        };

        Component &get_provider();

        Input::Action get_key(Input::Key key);
        Input::Action get_mouse_button(Input::MouseButton button);

        void set_mouse_pos_listener(OnMousePositionChange listener);
        void deregister_mouse_pos_listener(OnMousePositionChange listener);

        void set_mouse_button_listener(OnMouseButton listener);
        void deregister_mouse_button_listener(OnMouseButton listener);

        void set_scroll_listener(OnScroll listener);
        void deregister_scroll_listener(OnScroll listener);

        void set_key_listener(OnKey listener);
        void deregister_key_listener(OnKey listener);

    } // namespace InputSystem

    template <>
    inline const char *get_struct_name<InputSystem::Component>()
    {
        return "InputSystem";
    }

} // namespace Vultr
