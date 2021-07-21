#include <ecs/system/system_provider.hpp>
#include <ecs/entity/entity.hpp>

namespace Vultr
{
    void system_provider_on_create_entity(Engine *e, SystemProvider &p, Entity entity)
    {
        p.entities.insert(entity);
        if (p.on_create_entity != nullptr)
            p.on_create_entity(e, entity);
    }

    void system_provider_on_destroy_entity(Engine *e, SystemProvider &p, Entity entity)
    {
        if (p.entities.find(entity) == p.entities.end())
            return;
        p.entities.erase(entity);
        if (p.on_destroy_entity != nullptr)
            p.on_destroy_entity(e, entity);
    }
} // namespace Vultr
