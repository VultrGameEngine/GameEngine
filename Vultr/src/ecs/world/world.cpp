#include <ecs/world/world.hpp>
#include <ecs/world/internal_world.hpp>
#include <helpers/file.h>
#include <json/json.hpp>
#include <engine.hpp>
#include <fstream>

namespace Vultr
{
    World *new_world(const ComponentRegistry &r)
    {
        World *world = new InternalWorld();
        for (auto [type, data] : r.components)
        {
            auto *array = data.component_array_constructor();
            world->component_manager.component_arrays[type] = array;
        }
        return world;
    }

    static bool component_registries_match(const ComponentRegistry &r_old, const ComponentRegistry &r)
    {
        // Make sure that every component in old registry has the same type as new component registry
        // this will allow for new components to be added and save files will be compatible
        // will return false if component names are changed
        for (auto [type, name] : r_old.component_type_to_name)
        {
            if (r.component_type_to_name.find(type) == r.component_type_to_name.end() || r.component_type_to_name.at(type) != name)
                return false;
        }
        return true;
    }

    static Signature convert_signature(Signature signature, const ComponentRegistry &r_old, const ComponentRegistry &r)
    {
        Signature new_sig;
        for (ComponentType i = 0; i < r_old.next_component_type; i++)
        {
            // If the old signature has this component turned off then there's nothing to do

            if (!signature[i])
                continue;
            std::string component_name = r_old.component_type_to_name.at(i);

            // If the component no longer exists, then there's nothing to turn on
            if (r.component_name_to_type.find(component_name) == r.component_name_to_type.end())
                continue;
            ComponentType new_component_type = r.component_name_to_type.at(component_name);
            new_sig.set(new_component_type);
        }
        return new_sig;
    }

    static void entity_manager_from_json(const json &j, EntityManager &m, const ComponentRegistry &r_old, const ComponentRegistry &r)
    {
        bool need_to_convert = component_registries_match(r_old, r);
        m.living_entites = j["living_entites"].get<std::set<Entity>>();
        m.living_entity_count = j["living_entity_count"].get<u32>();
        for (Entity e : m.living_entites)
        {
            Signature old_entity_signature = j["signatures"][std::to_string(e)];
            if (!need_to_convert)
            {
                m.signatures[e] = old_entity_signature;
            }
            else
            {
                m.signatures[e] = convert_signature(old_entity_signature, r_old, r);
            }
        }

        // Clear the queue because it was likely constructed incorrectly
        std::queue<Entity> empty;
        std::swap(m.available_entities, empty);

        // Fill available entities with those not found in the living entities set
        for (Entity entity = 1; entity < MAX_ENTITIES; ++entity)
        {
            if (m.living_entites.find(entity) == m.living_entites.end())
            {
                m.available_entities.push(entity);
            }
        }
    }

    World *load_world(const File &file, const ComponentRegistry &r)
    {
        World *world = new_world(r);

        std::ifstream i;
        i.open(file.GetPath());

        json world_saved_json;
        i >> world_saved_json;
        component_manager_from_json(world_saved_json["ComponentManager"], world->component_manager, engine_global().component_registry);
        ComponentRegistry old_registry = world_saved_json["ComponentRegistry"];
        entity_manager_from_json(world_saved_json["EntityManager"], world->entity_manager, old_registry, engine_global().component_registry);
        return world;
    }

    void save_world(World *_world, const File &out)
    {
        std::ofstream o;
        o.open(out.GetPath());
        InternalWorld *world = static_cast<InternalWorld *>(_world);
        json component_manager_json;
        component_manager_to_json(component_manager_json, world_get_component_manager(world), engine_global().component_registry);
        json component_registry_json = engine_global().component_registry;
        json entity_manager_json = world_get_entity_manager(world);
        json final_output = {
            {"ComponentRegistry", component_registry_json},
            {"ComponentManager", component_manager_json},
            {"EntityManager", entity_manager_json},
        };
        o << std::setw(4) << final_output;
        o.close();
    }

    void destroy_world(World *world)
    {
        destroy_component_manager(world->component_manager);
        delete world;
    }

    Entity create_entity(World *world)
    {
        return entity_manager_create_entity(world->entity_manager);
    }

    void destroy_entity(World *world, Entity entity)
    {
        entity_manager_destroy_entity(world->entity_manager, entity);
    }

    std::set<Entity> get_entities(World *world, Signature signature)
    {
        return entity_manager_get_entities(world->entity_manager, signature);
    }

    Signature get_entity_signature(World *world, Entity entity)
    {
        return entity_manager_get_signature(world->entity_manager, entity);
    }

    // Getters for World
    EntityManager &world_get_entity_manager(World *world)
    {
        return world->entity_manager;
    }
    ComponentManager &world_get_component_manager(World *world)
    {
        return world->component_manager;
    }
    SystemManager &world_get_system_manager(World *world)
    {
        return world->system_manager;
    }
} // namespace Vultr
