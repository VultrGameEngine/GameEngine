﻿#include <ecs/component/component_manager.hpp>
#include <string.h>

namespace Vultr
{

    void component_manager_copy(ComponentManager &copy, const ComponentManager &other)
    {
        for (auto &[type, arr] : other.component_arrays)
        {
            copy.component_arrays[type] = arr->Copy();
        }
    }

    void component_manager_entity_destroyed(ComponentManager &manager, Entity entity)
    {
        // Notify all component arrays that an entity has been destroyed
        for (auto const &pair : manager.component_arrays)
        {
            auto const &component = pair.second;
            component->EntityDestroyed(entity);
        }
    }

    // {
    //    "Components": {
    //        "ComponentName1": {
    //        	... list of component jsons ...
    //        },
    //        "ComponentName2": {
    //       	... list of component jsons ...
    //        },
    //
    //    }
    // }
    //
    //

    void destroy_component_manager(ComponentManager &m)
    {
        for (auto [type, array] : m.component_arrays)
        {
            delete array;
        }
    }
    void component_manager_remove_component(ComponentManager &manager, ComponentType type)
    {
        delete manager.component_arrays[type];
        manager.component_arrays.erase(type);
    }

} // namespace Vultr
