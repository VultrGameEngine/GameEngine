#pragma once
#include <core/system_providers/input_system_provider.h>

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
typedef bool (*OnHoverCallback)(HoverEvent, void *);
struct OnHover
{
  public:
    OnHover(OnHoverCallback p_callback, void *p_caller)
        : callback(p_callback), caller(p_caller)
    {
    }

    OnHover() : callback(nullptr), caller(nullptr)
    {
    }

    bool Call(HoverEvent event)
    {
        if (IsValid())
            return callback(event, caller);
        return false;
    }

    bool IsValid()
    {
        return callback != nullptr && caller != nullptr;
    }

  private:
    OnHoverCallback callback = nullptr;
    void *caller = nullptr;
};
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
typedef bool (*OnUnhoverCallback)(UnhoverEvent, void *);

struct OnUnhover
{
  public:
    OnUnhover(OnUnhoverCallback p_callback, void *p_caller)
        : callback(p_callback), caller(p_caller)
    {
    }

    OnUnhover() : callback(nullptr), caller(nullptr)
    {
    }

    bool Call(UnhoverEvent event)
    {
        if (IsValid())
            return callback(event, caller);
        return false;
    }

    bool IsValid()
    {
        return callback != nullptr && caller != nullptr;
    }

  private:
    OnUnhoverCallback callback = nullptr;
    void *caller = nullptr;
};
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
typedef bool (*OnMouseDownCallback)(MouseDownEvent, void *);

struct OnMouseDown
{
  public:
    OnMouseDown(OnMouseDownCallback p_callback, void *p_caller)
        : callback(p_callback), caller(p_caller)
    {
    }

    OnMouseDown() : callback(nullptr), caller(nullptr)
    {
    }

    bool Call(MouseDownEvent event)
    {
        if (IsValid())
            return callback(event, caller);
        return false;
    }

    bool IsValid()
    {
        return callback != nullptr && caller != nullptr;
    }

  private:
    OnMouseDownCallback callback = nullptr;
    void *caller = nullptr;
};
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

typedef bool (*OnMouseUpCallback)(MouseUpEvent, void *);

struct OnMouseUp
{
  public:
    OnMouseUp(OnMouseUpCallback p_callback, void *p_caller)
        : callback(p_callback), caller(p_caller)
    {
    }

    OnMouseUp() : callback(nullptr), caller(nullptr)
    {
    }

    bool Call(MouseUpEvent event)
    {
        if (IsValid())
            return callback(event, caller);
        return false;
    }

    bool IsValid()
    {
        return callback != nullptr && caller != nullptr;
    }

  private:
    OnMouseUpCallback callback = nullptr;
    void *caller = nullptr;
};
} // namespace Vultr
