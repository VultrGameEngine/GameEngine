// [ComponentManager] is in charge of talking to all of the [ComponentArrays]

#pragma once
#include "component.hpp"
#include "component_array.hpp"
#include "../entity/entity.hpp"
#include <unordered_map>
#include <memory>
#include <iostream>

class ComponentManager
{
public:
    template <typename T>
    void RegisterComponent()
    {
        const char *type_name = typeid(T).name();

        assert(component_types.find(type_name) == component_types.end() && "Registered component type more than once");

        // Add the component type to the map
        component_types.insert({type_name, next_component_type});

        // Create a component array pointer and add it to the component array map
        component_arrays.insert({type_name, std::make_shared<ComponentArray<T>>()});

        // Increment the next component type
        ++next_component_type;
    }

    template <typename T>
    ComponentType GetComponentType()
    {
        const char *type_name = typeid(T).name();

        assert(component_types.find(type_name) != component_types.end() && "Component not registered before use");

        // Return this component's type used for creating signatures
        return component_types[type_name];
    }

    template <typename T>
    void AddComponent(Entity entity, T component)
    {
        GetComponentArray<T>()->InsertData(entity, component);
    }

    template <typename T>
    void RemoveComponent(Entity entity)
    {
        GetComponentArray<T>()->RemoveData(entity);
    }

    template <typename T>
    T &GetComponent(Entity entity)
    {
        return GetComponentArray<T>()->GetData(entity);
    }

    template <typename T>
    std::shared_ptr<ComponentArray<T>> GetComponents()
    {
        return GetComponentArray<T>();
    }

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
    // Map from type string pointer to a component type
    std::unordered_map<const char *, ComponentType> component_types{};

    // Map from type string poiner to a component array
    std::unordered_map<const char *, std::shared_ptr<IComponentArray>> component_arrays{};

    // The component type to be assigned to the next registered component starting at 0
    ComponentType next_component_type{};

    // Get the statically casted pointer to the ComponentArray of type T
    template <typename T>
    std::shared_ptr<ComponentArray<T>> GetComponentArray()
    {
        const char *type_name = typeid(T).name();

        assert(component_types.find(type_name) != component_types.end() && "Component not registered before use");

        return std::static_pointer_cast<ComponentArray<T>>(component_arrays[type_name]);
    }
};