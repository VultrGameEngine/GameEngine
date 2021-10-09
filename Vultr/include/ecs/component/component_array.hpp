// Interface is used by the ComponentManager to tell a ageneric ComponentArray
// that an entity has been destroyed
#pragma once
#include "../entity/entity.hpp"
#include <array>
#include <assert.h>
#include <memory>
#include <unordered_map>
#include <json/json_fwd.hpp>

namespace Vultr
{
    struct IComponentArray
    {
        virtual ~IComponentArray() = default;
        virtual bool HasData(Entity entity)
        {
            return false;
        }
        virtual void EntityDestroyed(Entity entity){};
        virtual void DuplicateData(Entity original, Entity new_entity){};
        virtual IComponentArray *Copy()
        {
            return nullptr;
        }
        virtual void InsertData(Entity entity, void *component){};
        virtual void *InternalGetData(Entity entity)
        {
            return nullptr;
        }
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

        void InsertData(Entity entity, void *component) override
        {
            T dereferenced = *static_cast<T *>(component);
            InsertData(entity, dereferenced);
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

        void *InternalGetData(Entity entity) override
        {
            T *component = GetDataUnsafe(entity);
            if (component == nullptr)
                return nullptr;
            T *data = new T(*component);
            return static_cast<void *>(data);
        }

        bool HasData(Entity entity) override
        {
            return entity_to_index_map.find(entity) != entity_to_index_map.end();
        }

        void DuplicateData(Entity original, Entity duplicate) override
        {
            assert(HasData(original) && !HasData(duplicate) && "Cannot duplicate because original does not exist or duplicate entity already exists");

            T component = GetData(original);
            InsertData(duplicate, component);
        }

        IComponentArray *Copy() override
        {
            ComponentArray<T> *copy = new ComponentArray<T>();
            copy->component_array = this->component_array;
            copy->entity_to_index_map = this->entity_to_index_map;
            copy->index_to_entity_map = this->index_to_entity_map;
            copy->size = this->size;
            return copy;
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

        void to_json(json &j) const override;

        void from_json(const json &j) override;

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

    // void to_json(json &j, const IComponentArray *a);

} // namespace Vultr
