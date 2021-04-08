// [ComponentManager] is in charge of talking to all of the [ComponentArrays]

#pragma once
#include "component.hpp"
#include "component_array.hpp"
#include "component_registry.hpp"
#include <iostream>
#include <memory>
#include <unordered_map>
#include <cereal/types/memory.hpp>
#include <cereal/types/unordered_map.hpp>

class ComponentManager
{
  public:
    template <typename T> void AddComponent(Entity entity, T component)
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

    template <typename T> T *GetComponentUnsafe(Entity entity)
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

    template <class Archive> void serialize(Archive &archive)
    {
        archive(component_arrays); // serialize things by passing them to the archive
    }

  private:
    // Map from type string poiner to a component array
    std::unordered_map<std::string, std::shared_ptr<IComponentArray>>
        component_arrays{};

    // Get the statically casted pointer to the ComponentArray of type T
    template <typename T> std::shared_ptr<ComponentArray<T>> GetComponentArray()
    {
        const char *type_name = typeid(T).name();

        // assert(Vultr::ComponentRegistry::ComponentRegistered(type_name) &&
        //        "Component not registered before use");
        if (component_arrays.find(type_name) == component_arrays.end())
        {
            component_arrays.insert(
                {type_name, std::make_shared<ComponentArray<T>>()});
        }

        return std::static_pointer_cast<ComponentArray<T>>(
            component_arrays[type_name]);
    }
};
