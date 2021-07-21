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
            Input::MousePos mouse_pos = Input::MousePos(0, 0);
            Input::MousePos scene_mouse_pos = Input::MousePos(0, 0);

            // Internal
            std::queue<Vec2> internal_scroll_queue;

            std::vector<OnMousePositionChange> mouse_position_listeners;
            std::vector<OnMouseButton> mouse_button_listeners;
            std::vector<OnScroll> scroll_listeners;
            std::vector<OnKey> key_listeners;
            std::vector<OnCharacter> character_listeners;
        };

        Component &get_provider(Engine *e);

        Input::Action get_key(Engine *e, Input::Key key);
        Input::Action get_mouse_button(Engine *e, Input::MouseButton button);

        bool mouse_is_on_screen(Engine *e);
        bool mouse_is_on_screen(Engine *e, Vec2 pos);

        const Input::MousePos &get_mouse_position(Engine *e);
        const Input::MousePos &get_scene_mouse_position(Engine *e);

        void set_mouse_pos_listener(Engine *e, OnMousePositionChange listener);
        void deregister_mouse_pos_listener(Engine *e, OnMousePositionChange listener);

        void set_mouse_button_listener(Engine *e, OnMouseButton listener);
        void deregister_mouse_button_listener(Engine *e, OnMouseButton listener);

        void set_scroll_listener(Engine *e, OnScroll listener);
        void deregister_scroll_listener(Engine *e, OnScroll listener);

        void set_key_listener(Engine *e, OnKey listener);
        void deregister_key_listener(Engine *e, OnKey listener);

        void set_character_listener(Engine *e, OnCharacter listener);
        void deregister_character_listener(Engine *e, OnCharacter listener);

    } // namespace InputSystem

    template <>
    inline const char *get_struct_name<InputSystem::Component>()
    {
        return "InputSystem";
    }

} // namespace Vultr
