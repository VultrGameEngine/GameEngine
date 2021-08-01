#include <core/components/light_component.h>
#include <core/components/transform_component.h>
#include <core/systems/light_system.h>
#include <core/system_providers/light_system_provider.h>
#include <core/system_providers/shader_loader_system_provider.h>
#include <ecs/world/world.hpp>
#include <engine.hpp>

namespace Vultr::LightSystem
{
    void register_system(Engine *e)
    {
        Signature signature;
        signature.set(get_component_type<LightComponent>(e), true);
        signature.set(get_component_type<TransformComponent>(e), true);
        register_global_system<Component>(e, signature, nullptr, on_destroy_entity);

        auto &p = get_provider(e);
        p.internal_point_lights_uniform = new ShaderLoaderSystem::PointLightsUniform();
    }

    static void remove_extraneous_point_lights(Engine *e, Entity entity)
    {
        auto &p = get_provider(e);
        if (p.point_lights.find(entity) != p.point_lights.end())
        {
            p.point_lights.erase(entity);
        }
    }

    void update(Engine *e)
    {
        auto &p = get_provider(e);

        u16 point_light_index = 0;
        for (auto entity : p.entities)
        {
            auto &light_component = entity_get_component<LightComponent>(e, entity);
            switch (light_component.type)
            {
                case LightComponent::DirectionalLight:
                {
                    if (p.directional_light != INVALID_ENTITY && p.directional_light != entity)
                    {
                        fprintf(stderr, "Vultr only supports 1 directional light per scene!\n");
                    }
                    else
                    {
                        p.directional_light = entity;
                    }
                    remove_extraneous_point_lights(e, entity);
                    break;
                }
                case LightComponent::PointLight:
                {
                    p.point_lights.insert(entity);
                    auto &transform_component = entity_get_component<TransformComponent>(e, entity);

                    p.internal_point_lights_uniform->positions[point_light_index] = Vec4(transform_component.position, 1.0);
                    p.internal_point_lights_uniform->constants[point_light_index] = Vec4(light_component.constant);
                    p.internal_point_lights_uniform->linears[point_light_index] = Vec4(light_component.linear);
                    p.internal_point_lights_uniform->quadratics[point_light_index] = Vec4(light_component.quadratic);
                    p.internal_point_lights_uniform->ambients[point_light_index] = light_component.ambient.value / Vec4(255);
                    p.internal_point_lights_uniform->diffuses[point_light_index] = light_component.diffuse.value / Vec4(255);
                    p.internal_point_lights_uniform->speculars[point_light_index] = Vec4(light_component.specular);
                    p.internal_point_lights_uniform->intensities[point_light_index] = Vec4(light_component.intensity);
                    point_light_index++;

                    assert(point_light_index < MAX_POINT_LIGHTS && "Max number of point lights exceeded!");
                    break;
                }
                case LightComponent::SpotLight:
                {
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
        ShaderLoaderSystem::DirectionalLightUniform uniform = {};
        if (p.directional_light != INVALID_ENTITY)
        {
            auto [transform_component, light_component] = entity_get_components<TransformComponent, LightComponent>(e, p.directional_light);
            uniform = {
                .direction = Vec4(-transform_component.Up(), 0.0),
                .ambient = light_component.ambient.value / Vec4(255),
                .diffuse = light_component.diffuse.value / Vec4(255),
                .specular = light_component.specular,
                .intensity = light_component.intensity,
                .exists = 1,
            };
        }
        else
        {
            uniform.exists = 0;
        }
        ShaderLoaderSystem::set_directional_light_uniform(e, uniform);

        p.internal_point_lights_uniform->count = point_light_index;
        ShaderLoaderSystem::set_point_lights_uniform(e, *p.internal_point_lights_uniform);
    }

    void on_destroy_entity(Engine *e, Entity entity)
    {
        auto &provider = get_provider(e);
        if (provider.directional_light == entity)
        {
            provider.directional_light = INVALID_ENTITY;
        }
        if (provider.point_lights.find(entity) != provider.point_lights.end())
        {
            provider.point_lights.erase(entity);
        }
    }

} // namespace Vultr::LightSystem
