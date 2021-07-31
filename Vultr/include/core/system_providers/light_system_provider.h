#pragma once
#include <ecs/system/system_provider.hpp>
#include <engine.hpp>

namespace Vultr
{
    namespace ShaderLoaderSystem
    {
        struct PointLightsUniform;
    }

    namespace LightSystem
    {
        struct Component : public SystemProvider
        {
#define MAX_POINT_LIGHTS 512
            // In this engine, we will assume there is only one directional light in the scene for simplicity's sake. If a second directional light gets added, then we will throw an error
            Entity directional_light = INVALID_ENTITY;

            std::set<Entity> point_lights;

            ShaderLoaderSystem::PointLightsUniform *internal_point_lights_uniform;

            ~Component();
        };

        Component &get_provider(Engine *e);
    } // namespace LightSystem

    template <>
    inline const char *get_struct_name<LightSystem::Component>()
    {
        return "LightSystem";
    }
} // namespace Vultr
