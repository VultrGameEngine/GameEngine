#include <ecs/system/system_provider.hpp>
#include <ecs/entity/entity.hpp>

void SystemProvider::DestroyEntity(Entity entity)
{
    if (entities.find(entity) == entities.end())
        return;
    entities.erase(entity);
    OnDestroyEntity(entity);
}
void SystemProvider::CreateEntity(Entity entity)
{
    entities.insert(entity);
    OnCreateEntity(entity);
}
bool SystemProvider::Match(Signature other)
{
    return (other & signature) == signature;
}
