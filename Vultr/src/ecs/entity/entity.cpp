#include <ecs/entity/entity.hpp>
#include <ecs/world/world.hpp>

Entity Entity::New()
{
    return World::Get()->CreateEntity();
}

Signature Entity::GetSignature()
{
    return World::Get()->GetSignature(*this);
}
