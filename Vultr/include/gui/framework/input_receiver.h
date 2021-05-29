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
            OnHoverCallback on_hover;
            OnUnhoverCallback on_unhover;
            OnMouseDownCallback on_mouse_down;
            OnMouseUpCallback on_mouse_up;
            OnScrollCallback on_scroll;
            Vec2 top_left;
            Vec2 bottom_right;

          private:
            struct Params
            {
                OnHoverCallback on_hover;
                OnUnhoverCallback on_unhover;
                OnMouseDownCallback on_mouse_down;
                OnMouseUpCallback on_mouse_up;
                OnScrollCallback on_scroll;
            };

            struct State
            {
                bool is_hovered = false;
                bool mouse_down = false;
            } state;

          public:
            InputReceiver(Params params)
                : on_hover(params.on_hover), on_unhover(params.on_unhover),
                  on_mouse_down(params.on_mouse_down),
                  on_mouse_up(params.on_mouse_up), bottom_right(Vec2(1, -1)),
                  top_left(-1, 1), on_scroll(params.on_scroll)
            {
            }

            bool InBounds(Vec2 pos)
            {
                return pos.x > top_left.x && pos.x < bottom_right.x &&
                       pos.y < top_left.y && pos.y > bottom_right.y;
            }

            bool ReceiveMouseEvent(Input::MouseInputEvent event)
            {
                if (InBounds(event.pos))
                {
                    if (!state.is_hovered)
                    {
                        if (on_hover && on_hover(HoverEvent(event)))
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
                else if (state.is_hovered && on_unhover)
                {
                    if (on_unhover(UnhoverEvent(event)))
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
                        if (!on_mouse_down)
                        {
                        }
                        // On mouse down
                        if (!state.mouse_down)
                        {
                            if (on_mouse_down &&
                                on_mouse_down(MouseDownEvent(event)))
                            {
                                state.mouse_down = true;
                                return true;
                            }
                            // If the receiver didn't get this for some reason, then
                            // it didn't go through properly
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
                        if (on_mouse_up && on_mouse_up(MouseUpEvent(event)))
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
            bool ReceiveScrollEvent(Input::ScrollInputEvent event)
            {
                if (InBounds(event.pos))
                {
                    if (on_scroll && on_scroll(ScrollEvent(event)))
                    {
                        return true;
                    }
                }
                return false;
            }

            ~InputReceiver()
            {
            }
        };
    } // namespace GUI
} // namespace Vultr
