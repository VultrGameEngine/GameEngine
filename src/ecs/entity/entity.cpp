#include <ecs/entity/entity.hpp>
#include <ecs/world/world.hpp>

Entity Entity::New()
{
    return World::CreateEntity();
}
