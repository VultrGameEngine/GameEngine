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
    bool receive_mouse_event(Input::MouseInputEvent event);
    bool receive_mouse_button_event(Input::MouseButtonInputEvent event);
    bool receive_scroll_event(Input::ScrollInputEvent event);

} // namespace Vultr::GUISystem
