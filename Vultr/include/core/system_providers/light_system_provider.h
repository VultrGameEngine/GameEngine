#pragma once
#include <ecs/system/system_provider.hpp>
#include <engine.hpp>

namespace Vultr
{
    namespace LightSystem
    {
        struct Component : public SystemProvider
        {
            Entity light = INVALID_ENTITY;
        };

        Component &get_provider();
    } // namespace LightSystem

    template <>
    inline const char *get_struct_name<LightSystem::Component>()
    {
        return "LightSystem";
    }
} // namespace Vultr
