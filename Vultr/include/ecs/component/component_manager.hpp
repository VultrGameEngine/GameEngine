// [ComponentManager] is in charge of talking to all of the [ComponentArrays]

#pragma once
#include "component.hpp"
#include "component_array.hpp"
#include "component_registry.hpp"
#include <iostream>
#include <memory>
#include <unordered_map>

namespace Vultr
{
    struct ComponentManager
    {
        ComponentManager() = default;
        // Map from type string poiner to a component array
        std::unordered_map<ComponentType, IComponentArray *> component_arrays{};
    };

    void component_manager_entity_destroyed(ComponentManager &manager, Entity entity);

    // Get the statically casted pointer to the ComponentArray of type T
    template <typename T>
    ComponentArray<T> *component_manager_get_component_array(ComponentManager &manager, ComponentType type)
    {
        // If the component manager does not have that component type then insert a
        // new component array
        if (manager.component_arrays.find(type) == manager.component_arrays.end())
        {
            manager.component_arrays.insert({type, new ComponentArray<T>()});
        }

        return static_cast<ComponentArray<T> *>(manager.component_arrays[type]);
    }

    void component_manager_to_json(json &j, const ComponentManager &m, const ComponentRegistry &r);

    void component_manager_from_json(const json &j, ComponentManager &m, const ComponentRegistry &r);

    void destroy_component_manager(ComponentManager &manager);

} // namespace Vultr
