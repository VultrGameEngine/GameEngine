#pragma once
#include <ecs/system/system_provider.hpp>
#include <engine.hpp>

namespace Vultr
{
    namespace ControllerSystem
    {
        struct Component : public SystemProvider
        {
            GLFWwindow *window;
            bool focused = false;
        };

        Component &get_provider();
    } // namespace ControllerSystem
    template <>
    inline const char *get_struct_name<ControllerSystem::Component>()
    {
        return "ControllerSystem";
    }
} // namespace Vultr
