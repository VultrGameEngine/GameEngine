// Controller system handles input from the user translated to transform changes

#pragma once
#include <core/components/controller_component.h>
#include <core/components/transform_component.h>

namespace Vultr::ControllerSystem
{
    void register_system();
    void init(GLFWwindow *window);
    void update(float delta_time);
    void window_focus_callback(GLFWwindow *window, int focused);
} // namespace Vultr::ControllerSystem
