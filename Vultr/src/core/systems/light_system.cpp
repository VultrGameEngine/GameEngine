#include <core/components/light_component.h>
#include <core/components/transform_component.h>
#include <core/systems/light_system.h>
#include <core/system_providers/light_system_provider.h>
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
        for (auto entity : p.entities)
        {
            auto &light_component = entity_get_component<LightComponent>(e, entity);
            switch (light_component.type)
            {
                case LightComponent::DirectionalLight:
                {
                    if (p.directional_light != INVALID_ENTITY && p.directional_light != entity)
                    {
                        std::cout << "Vultr only supports 1 directional light per scene!";
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
