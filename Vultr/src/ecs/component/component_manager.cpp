#include <ecs/component/component_manager.hpp>

namespace Vultr
{

void EntityDestroyed(ComponentManager &manager, Entity entity)
{
    // Notify all component arrays that an entity has been destroyed
    for (auto const &pair : manager.component_arrays)
    {
        auto const &component = pair.second;
        component->EntityDestroyed(entity);
    }
}
} // namespace Vultr
