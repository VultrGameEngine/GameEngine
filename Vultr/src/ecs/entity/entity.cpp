#include <ecs/entity/entity.hpp>
#include <ecs/world/world.hpp>

Entity Entity::New()
{
    return World::CreateEntity();
}

Signature Entity::GetSignature()
{
    return World::GetSignature(*this);
}
