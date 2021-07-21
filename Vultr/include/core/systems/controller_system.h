// Controller system handles input from the user translated to transform changes

#pragma once
#include <engine.hpp>
#include <core/components/controller_component.h>
#include <core/components/transform_component.h>

namespace Vultr::ControllerSystem
{
    void register_system(Engine *e);
    void init(Engine *e, GLFWwindow *window);
    void update(Engine *e, float delta_time);
    void window_focus_callback(GLFWwindow *window, int focused);
} // namespace Vultr::ControllerSystem
