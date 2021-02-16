// [ComponentManager] is in charge of talking to all of the [ComponentArrays]

#pragma once
#include "component.hpp"
#include "component_array.hpp"
#include "../../core/component_renderer.h"
#include <iostream>
#include <memory>
#include <unordered_map>
#include <cereal/types/memory.hpp>
#include <cereal/types/unordered_map.hpp>

class ComponentManager
{
  public:
    template <typename T> void RegisterComponent(ComponentRender func)
    {
        const char *type_name = typeid(T).name();

        assert(component_types.find(type_name) == component_types.end() &&
               "Registered component type more than once");

        // Add the component type to the map
        component_types.insert({type_name, next_component_type});

        // Create a component array pointer and add it to the component array map
        component_arrays.insert({type_name, std::make_shared<ComponentArray<T>>()});

        // Add the component to the map
        component_renderers.insert({type_name, func});

        // Increment the next component type
        ++next_component_type;
    }

    template <typename T> ComponentType GetComponentType()
    {
        const char *type_name = typeid(T).name();

        assert(component_types.find(type_name) != component_types.end() &&
               "Component not registered before use");

        // Return this component's type used for creating signatures
        return component_types[type_name];
    }

    template <typename T>
    void AddComponent(Entity entity, std::shared_ptr<T> component)
    {
        GetComponentArray<T>()->InsertData(entity, component);
    }

    template <typename T> void RemoveComponent(Entity entity)
    {
        GetComponentArray<T>()->RemoveData(entity);
    }

    template <typename T> T &GetComponent(Entity entity)
    {
        return GetComponentArray<T>()->GetData(entity);
    }

    template <typename T> std::shared_ptr<T> GetComponentUnsafe(Entity entity)
    {
        return GetComponentArray<T>()->GetDataUnsafe(entity);
    }

    template <typename T> std::shared_ptr<ComponentArray<T>> GetComponents()
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

    void RenderEntityComponents(Entity entity)
    {
        for (auto const [type, render] : component_renderers)
        {
            render(entity);
        }
    }

    template <class Archive> void serialize(Archive &archive)
    {
        archive(component_arrays); // serialize things by passing them to the archive
    }

    // Get the statically casted pointer to the ComponentArray of type T
    template <typename T> std::shared_ptr<ComponentArray<T>> GetComponentArray()
    {
        const char *type_name = typeid(T).name();

        assert(component_types.find(type_name) != component_types.end() &&
               "Component not registered before use");

        return std::static_pointer_cast<ComponentArray<T>>(
            component_arrays[type_name]);
    }

  private:
    // Map from type string pointer to a component type
    std::unordered_map<const char *, ComponentType> component_types{};

    // Map from type string poiner to a component array
    std::unordered_map<std::string, std::shared_ptr<IComponentArray>>
        component_arrays{};

    // Map from type string pointer to a render function
    std::unordered_map<const char *, ComponentRender> component_renderers{};

    // The component type to be assigned to the next registered component starting at
    // 0
    ComponentType next_component_type{};
};
