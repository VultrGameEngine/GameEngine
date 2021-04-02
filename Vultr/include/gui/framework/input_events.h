#pragma once
#include <core/system_providers/input_system_provider.h>
#include <functional>

namespace Vultr
{

// On Hover
struct HoverEvent
{
    HoverEvent(Input::MouseInputEvent event)
        : pos(event.pos), mouse_down(event.mouse_down)
    {
    }
    glm::vec2 pos;
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
    UnhoverEvent(Input::MouseInputEvent event)
        : pos(event.pos), mouse_down(event.mouse_down)
    {
    }
    glm::vec2 pos;
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
    MouseDownEvent(Input::MouseButtonInputEvent event)
        : pos(event.pos), mouse_down(event.mouse_down)

    {
    }
    glm::vec2 pos;
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
    MouseUpEvent(Input::MouseButtonInputEvent event)
        : pos(event.pos), mouse_down(event.mouse_down)
    {
    }
    glm::vec2 pos;
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
struct ScrollEvent
{
    ScrollEvent(Input::ScrollInputEvent event)
        : scroll_amount(event.scroll_amount), pos(event.pos)
    {
    }
    glm::vec2 scroll_amount;
    glm::vec2 pos;
};
typedef std::function<bool(ScrollEvent)> OnScrollCallback;
} // namespace Vultr
