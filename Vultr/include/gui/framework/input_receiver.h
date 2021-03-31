#pragma once
#include <glm/glm.hpp>
#include <core/system_providers/input_system_provider.h>
#include "input_events.h"

namespace Vultr
{

namespace GUI
{
class InputReceiver
{
  public:
    OnHover on_hover;
    OnUnhover on_unhover;
    OnMouseDown on_mouse_down;
    OnMouseUp on_mouse_up;
    glm::vec2 top_left;
    glm::vec2 bottom_right;

  private:
    struct Params
    {
        OnHover on_hover;
        OnUnhover on_unhover;
        OnMouseDown on_mouse_down;
        OnMouseUp on_mouse_up;
    };

    struct State
    {
        bool is_hovered = false;
        bool mouse_down = false;
    } state;

  public:
    InputReceiver(Params params)
        : on_hover(params.on_hover), on_unhover(params.on_unhover),
          on_mouse_down(params.on_mouse_down), on_mouse_up(params.on_mouse_up)
    {
    }

    bool InBounds(glm::vec2 pos)
    {
        return pos.x > top_left.x && pos.x < bottom_right.x && pos.y < top_left.y &&
               pos.y > bottom_right.y;
    }

    bool ReceiveMouseEvent(Input::MouseInputEvent event)
    {
        if (InBounds(event.pos))
        {
            if (!state.is_hovered)
            {
                if (on_hover.IsValid() && on_hover.Call(HoverEvent(event)))
                {
                    state.is_hovered = true;
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
        else if (state.is_hovered && on_unhover.IsValid())
        {
            if (on_unhover.Call(UnhoverEvent(event)))
            {
                state.is_hovered = false;
                return true;
            }
            else
            {
                return false;
            }
        }
        return false;
    }

    bool ReceiveMouseButtonEvent(Input::MouseButtonInputEvent event)
    {
        if (event.mouse_down)
        {
            if (InBounds(event.pos))
            {
                if (!on_mouse_down.IsValid())
                {
                }
                // On mouse down
                if (!state.mouse_down)
                {
                    if (on_mouse_down.IsValid() &&
                        on_mouse_down.Call(MouseDownEvent(event)))
                    {
                        state.mouse_down = true;
                        return true;
                    }
                    // If the receiver didn't get this for some reason, then it
                    // didn't go through properly
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    return true;
                }
            }
            else
            {
                return false;
            }
        }
        else
        {
            // On mouse up
            if (state.mouse_down)
            {
                if (on_mouse_up.IsValid() && on_mouse_up.Call(MouseUpEvent(event)))
                {
                    state.mouse_down = false;
                    return true;
                }
                // If the receiver didn't get this for some reason, then it
                // didn't go through properly
                else
                {
                    return false;
                }
            }
            // We don't have an event here since this receiver already knows
            // the mouse is up
            else
            {
                return true;
            }
        }
    }

    ~InputReceiver()
    {
    }
};
} // namespace GUI
} // namespace Vultr
