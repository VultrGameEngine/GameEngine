#pragma once
#include <core/system_providers/input_system_provider.h>
#include <functional>
#include <input/input.h>

namespace Vultr
{

    // On Hover
    struct HoverEvent
    {
        HoverEvent(MousePositionEvent event) : pos(event.pos)
        {
            mouse_down = InputSystem::get_mouse_button(Input::MOUSE_LEFT);
        }
        Vec2 pos;
        bool mouse_down;
    };
    typedef std::function<bool(HoverEvent)> OnHoverCallback;
    //
    //
    //
    //
    //
    //
    //
    // On Unhover
    struct UnhoverEvent
    {
        UnhoverEvent(MousePositionEvent event) : pos(event.pos)
        {
            mouse_down = InputSystem::get_mouse_button(Input::MOUSE_LEFT);
        }
        Vec2 pos;
        bool mouse_down;
    };
    typedef std::function<bool(UnhoverEvent)> OnUnhoverCallback;
    //
    //
    //
    //
    //
    //
    //
    //
    // On mouse down
    struct MouseDownEvent
    {
        MouseDownEvent(MouseButtonEvent event) : pos(event.pos), mouse_down(event.action)

        {
        }
        Vec2 pos;
        bool mouse_down;
    };
    typedef std::function<bool(MouseDownEvent)> OnMouseDownCallback;
    //
    //
    //
    //
    //
    //
    // On mouse up
    struct MouseUpEvent
    {
        MouseUpEvent(MouseButtonEvent event) : pos(event.pos), mouse_down(event.action)
        {
        }
        Vec2 pos;
        bool mouse_down;
    };

    typedef std::function<bool(MouseUpEvent)> OnMouseUpCallback;

    //
    //
    //
    //
    //
    //
    // On scroll
    typedef std::function<bool(ScrollEvent)> OnScrollCallback;
} // namespace Vultr
