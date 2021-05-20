// Interface is used by the ComponentManager to tell a ageneric ComponentArray
// that an entity has been destroyed
#pragma once
#include "../entity/entity.hpp"
#include <array>
#include <assert.h>
#include <memory>
#include <unordered_map>
#include <json/json.hpp>

namespace Vultr
{
    struct IComponentArray
    {
      public:
        virtual ~IComponentArray() = default;
        virtual void EntityDestroyed(Entity entity){};
        virtual void to_json(json &j) const {};
        virtual void from_json(const json &j){};
    };

    template <typename T>
    struct ComponentArray : public IComponentArray
    {
      public:
        void InsertData(Entity entity, T component)
        {
            assert(entity_to_index_map.find(entity) == entity_to_index_map.end() && "Component added to same entity more than once");

            // Create a new index
            // This is set to size because that is the logical next index
            size_t new_index = size;

            // Create the relations between the entity ID and the new_index in the
            // component array
            entity_to_index_map[entity] = new_index;
            index_to_entity_map[new_index] = entity;

            // Set the component in the component_array
            component_array[new_index] = component;

            ++size;
        }

        void RemoveData(Entity entity)
        {
            assert(entity_to_index_map.find(entity) != entity_to_index_map.end() && "Attempting to remove nonexistent component");

            // Get the index for the specified entity ID
            size_t index_of_removed_entity = entity_to_index_map[entity];

            // Get the index of the last element of the component array
            size_t index_of_last_element = size - 1;

            // Move the last element of the component array into the removed entity's
            // index
            component_array[index_of_removed_entity] = component_array[index_of_last_element];

            // Update the maps for the newly moved element
            Entity entity_of_last_element = index_to_entity_map[index_of_last_element];
            entity_to_index_map[entity_of_last_element] = index_of_removed_entity;
            index_to_entity_map[index_of_removed_entity] = entity_of_last_element;

            // Remove the entity requested from the maps
            entity_to_index_map.erase(entity);
            index_to_entity_map.erase(index_of_last_element);

            --size;
        }

        T &GetData(Entity entity)
        {
            assert(entity_to_index_map.find(entity) != entity_to_index_map.end() && "Attempting to retreive data from nonexistent component");

            return component_array[entity_to_index_map[entity]];
        }

        bool HasData(Entity entity)
        {
            return entity_to_index_map.find(entity) != entity_to_index_map.end();
        }

        T *GetDataUnsafe(Entity entity)
        {
            if (entity_to_index_map.find(entity) == entity_to_index_map.end())
                return nullptr;
            return &component_array[entity_to_index_map[entity]];
        }

        void EntityDestroyed(Entity entity) override
        {
            if (entity_to_index_map.find(entity) != entity_to_index_map.end())
            {
                RemoveData(entity);
            }
        }

        void to_json(json &j) const override
        {
            j["entity_to_index_map"] = entity_to_index_map;
            j["index_to_entity_map"] = index_to_entity_map;
            j["size"] = size;
            for (int i = 0; i < size; i++)
            {
                j["component_array"].push_back(component_array[i]);
            }
        }

        void from_json(const json &j) override
        {
            if (j.find("component_array") != j.end())
            {
                for (auto &component : j["component_array"].items())
                {
                    component_array[atoi(component.key().c_str())] = component.value();
                }
            }
            if (j.find("index_to_entity_map") != j.end())
            {
                index_to_entity_map = j["index_to_entity_map"].get<std::unordered_map<size_t, Entity>>();
            }
            if (j.find("entity_to_index_map") != j.end())
            {
                entity_to_index_map = j["entity_to_index_map"].get<std::unordered_map<Entity, size_t>>();
            }
            size = j["size"].get<size_t>();
        }

      private:
        // Packed array of components
        // Set to be of maximum length of the maximum number of entities
        // each entity has a unique spot
        std::array<T, MAX_ENTITIES> component_array{};

        // Map from entity ID to an array index;
        std::unordered_map<Entity, size_t> entity_to_index_map{};

        // Map from array index to entity ID
        std::unordered_map<size_t, Entity> index_to_entity_map{};

        // Total size of valid entries in the array
        size_t size{};
    };

    inline void to_json(json &j, const IComponentArray *a)
    {
        a->to_json(j);
    }

} // namespace Vultr
