// [ComponentManager] is in charge of talking to all of the [ComponentArrays]

#pragma once
#include "component.hpp"
#include "component_array.hpp"
#include "component_registry.hpp"
#include <iostream>
#include <memory>
#include <unordered_map>

class World;
class ComponentManager
{
  public:
    void EntityDestroyed(Entity entity)
    {
        // Notify all component arrays that an entity has been destroyed
        for (auto const &pair : component_arrays)
        {
            auto const &component = pair.second;
            component->EntityDestroyed(entity);
        }
    }

  private:
    // Map from type string poiner to a component array
    std::unordered_map<std::string, std::shared_ptr<IComponentArray>>
        component_arrays{};

    // Get the statically casted pointer to the ComponentArray of type T
    template <typename T> std::shared_ptr<ComponentArray<T>> GetComponentArray()
    {
        const char *type_name = typeid(T).name();

        if (component_arrays.find(type_name) == component_arrays.end())
        {
            component_arrays.insert(
                {type_name, std::make_shared<ComponentArray<T>>()});
        }

        return std::static_pointer_cast<ComponentArray<T>>(
            component_arrays[type_name]);
    }
    friend World;
    friend Entity;
};
