#pragma once
#include <core/system_providers/gui_system_provider.h>
#include <gui/framework/window.h>
#include <core/system_providers/input_system_provider.h>
#include <core/models/update_tick.h>

namespace Vultr::GUISystem
{
    void register_system();
    void init(GUI::Window *window);
    void update(UpdateTick tick);
    bool receive_mouse_position_event(MousePositionEvent event);
    bool receive_mouse_button_event(MouseButtonEvent event);
    bool receive_key_event(KeyEvent event);
    bool receive_scroll_event(ScrollEvent event);
} // namespace Vultr::GUISystem
