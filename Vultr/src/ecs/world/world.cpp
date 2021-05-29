#include <ecs/world/world.hpp>
#include <ecs/world/internal_world.hpp>
#include <core/core_component_serialization.h>
#include <helpers/file.h>
#include <json/json.hpp>
#include <json/glm_serializer.hpp>
#include <engine.hpp>
#include <fstream>

void nlohmann::adl_serializer<Vultr::Signature>::to_json(json &j, const Vultr::Signature &v)
{
    for (s16 i = 0; i < v.size(); i++)
    {
        j[i] = (int)v[i];
    }
}

void nlohmann::adl_serializer<Vultr::Signature>::from_json(const json &j, Vultr::Signature &v)
{
    for (s16 i = 0; i < j.size(); i++)
    {
        if (i >= Vultr::MAX_COMPONENTS)
        {
            printf("WARNING LOST DATA IN SIGNATURE DUE TO REDUCED MAX COMPONENTS");
            break;
        }
        v.set(i, j[i].get<int>() == 1);
    }
}

#define TOJSON(prop) j[#prop] = c.prop
#define FROMJSON(prop, type)                                                                                                                                                                                          \
    if (j.contains(#prop))                                                                                                                                                                                            \
    {                                                                                                                                                                                                                 \
        c.prop = j[#prop].get<type>();                                                                                                                                                                                \
    }

namespace Vultr
{

    void to_json(json &j, const CameraComponent &c)
    {
        TOJSON(enabled);
        TOJSON(fov);
        TOJSON(znear);
        TOJSON(zfar);
    }
    void from_json(const json &j, CameraComponent &c)
    {
        FROMJSON(enabled, bool)
        FROMJSON(fov, f32)
        FROMJSON(znear, f32)
        FROMJSON(zfar, f32)
    }
    void to_json(json &j, const ControllerComponent &c)
    {
        TOJSON(sens);
    }
    void from_json(const json &j, ControllerComponent &c)
    {
        FROMJSON(sens, f32)
    }
    void to_json(json &j, const LightComponent &c)
    {
        TOJSON(type);
        TOJSON(ambient);
        TOJSON(diffuse);
        TOJSON(specular);
        TOJSON(constant);
        TOJSON(linear);
        TOJSON(quadratic);
    }
    void from_json(const json &j, LightComponent &c)
    {
        FROMJSON(type, u8)
        FROMJSON(ambient, Color)
        FROMJSON(diffuse, Color)
        FROMJSON(specular, f32)
        FROMJSON(constant, f32)
        FROMJSON(linear, f32)
        FROMJSON(quadratic, f32)
    }
    void to_json(json &j, const SkyBoxComponent &c)
    {
        TOJSON(identifier);
    }
    void from_json(const json &j, SkyBoxComponent &c)
    {
        FROMJSON(identifier, std::string);
    }
    void to_json(json &j, const StaticMeshComponent &c)
    {
        TOJSON(source);
    }
    void from_json(const json &j, StaticMeshComponent &c)
    {
        FROMJSON(source, Vultr::ModelSource)
    }
    void to_json(json &j, const TransformComponent &c)
    {
        TOJSON(position);
        TOJSON(rotation);
        TOJSON(scale);
    }
    void from_json(const json &j, TransformComponent &c)
    {
        FROMJSON(position, Vec3);
        FROMJSON(rotation, Quat);
        FROMJSON(scale, Vec3);
    }
    void to_json(json &j, const MaterialComponent &c)
    {
        TOJSON(shader_source);
        TOJSON(textures);
        TOJSON(vec3s);
        TOJSON(vec4s);
        TOJSON(colors);
        TOJSON(ints);
        TOJSON(floats);
    }
    void from_json(const json &j, MaterialComponent &c)
    {
        FROMJSON(shader_source, Vultr::ShaderSource)
        FROMJSON(textures, std::vector<MaterialComponent::TexturePair>)
        if (j.contains("vec3s"))
        {
            c.vec3s = j["vec3s"].get<std::unordered_map<std::string, Vec3>>();
        }
        if (j.contains("vec4s"))
        {
            c.vec4s = j["vec4s"].get<std::unordered_map<std::string, Vec4>>();
        }
        if (j.contains("colors"))
        {
            c.colors = j["colors"].get<std::unordered_map<std::string, Color>>();
        }
        if (j.contains("ints"))
        {
            c.ints = j["ints"].get<std::unordered_map<std::string, s32>>();
        }
        if (j.contains("floats"))
        {
            c.floats = j["floats"].get<std::unordered_map<std::string, f32>>();
        }
    }
    void to_json(json &j, const MaterialComponent::TexturePair &c)
    {
        TOJSON(file);
        TOJSON(slot);
        TOJSON(name);
    }
    void from_json(const json &j, MaterialComponent::TexturePair &c)
    {
        FROMJSON(file, Vultr::TextureSource)
        FROMJSON(slot, u16)
        FROMJSON(name, std::string)
    }
    void to_json(json &j, const Color &c)
    {
        TOJSON(value);
    }
    void from_json(const json &j, Color &c)
    {
        FROMJSON(value, Vec4)
    }

#define JSONCOMPONENTARRAY(T)                                                                                                                                                                                         \
    template <>                                                                                                                                                                                                       \
    void Vultr::ComponentArray<T>::to_json(json &j) const                                                                                                                                                             \
    {                                                                                                                                                                                                                 \
        j["entity_to_index_map"] = entity_to_index_map;                                                                                                                                                               \
        j["index_to_entity_map"] = index_to_entity_map;                                                                                                                                                               \
        j["size"] = size;                                                                                                                                                                                             \
        for (int i = 0; i < size; i++)                                                                                                                                                                                \
        {                                                                                                                                                                                                             \
            j["component_array"].push_back(component_array[i]);                                                                                                                                                       \
        }                                                                                                                                                                                                             \
    }                                                                                                                                                                                                                 \
    template <>                                                                                                                                                                                                       \
    void Vultr::ComponentArray<T>::from_json(const json &j)                                                                                                                                                           \
    {                                                                                                                                                                                                                 \
        if (j.find("component_array") != j.end())                                                                                                                                                                     \
        {                                                                                                                                                                                                             \
            for (auto &component : j["component_array"].items())                                                                                                                                                      \
            {                                                                                                                                                                                                         \
                component_array[atoi(component.key().c_str())] = component.value();                                                                                                                                   \
            }                                                                                                                                                                                                         \
        }                                                                                                                                                                                                             \
        if (j.find("index_to_entity_map") != j.end())                                                                                                                                                                 \
        {                                                                                                                                                                                                             \
            index_to_entity_map = j["index_to_entity_map"].get<std::unordered_map<size_t, Entity>>();                                                                                                                 \
        }                                                                                                                                                                                                             \
        if (j.find("entity_to_index_map") != j.end())                                                                                                                                                                 \
        {                                                                                                                                                                                                             \
            entity_to_index_map = j["entity_to_index_map"].get<std::unordered_map<Entity, size_t>>();                                                                                                                 \
        }                                                                                                                                                                                                             \
        size = j["size"].get<size_t>();                                                                                                                                                                               \
    }

    JSONCOMPONENTARRAY(CameraComponent)
    JSONCOMPONENTARRAY(ControllerComponent)
    JSONCOMPONENTARRAY(LightComponent)
    JSONCOMPONENTARRAY(SkyBoxComponent)
    JSONCOMPONENTARRAY(StaticMeshComponent)
    JSONCOMPONENTARRAY(TransformComponent)
    JSONCOMPONENTARRAY(MaterialComponent)

    void to_json(json &j, const Vultr::IComponentArray *a)
    {
        a->to_json(j);
    }
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

    void to_json(json &j, const File &f)
    {
        j["path"] = f.path;
        j["expected_extensions"] = f.expected_extensions;
    }
    void from_json(const json &j, File &f)
    {
        f.path = j["path"].get<std::string>();
        if (j.contains("expected_extensions"))
            f.expected_extensions = j["expected_extensions"].get<std::vector<std::string>>();
    }

    void component_manager_to_json(json &j, const ComponentManager &m, const ComponentRegistry &r)
    {
        json component_arrays;
        for (auto [type, array] : m.component_arrays)
        {
            component_arrays[r.component_type_to_name.at(type)] = array;
        }
        j["Components"] = component_arrays;
    }

    void component_manager_from_json(const json &j, ComponentManager &m, const ComponentRegistry &r)
    {
        for (auto &element : j["Components"].items())
        {
            std::string component_name = std::string(element.key().c_str());
            if (r.component_name_to_type.find(component_name.c_str()) == r.component_name_to_type.end())
            {
                continue;
            }
            ComponentType type = r.component_name_to_type.at(component_name.c_str());
            if (m.component_arrays.find(type) != m.component_arrays.end())
            {
                m.component_arrays[type]->from_json(element.value());
            }
        }
    }

    void to_json(json &j, const ComponentRegistry &r)
    {
        j["component_name_to_type"] = r.component_name_to_type;
        j["next_component_type"] = r.next_component_type;
    }

    void from_json(const json &j, ComponentRegistry &r)
    {
        r.component_name_to_type = j["component_name_to_type"].get<std::unordered_map<std::string, ComponentType>>();
        for (auto [name, type] : r.component_name_to_type)
        {
            r.component_type_to_name[type] = name;
        }
        r.next_component_type = j["next_component_type"].get<ComponentType>();
    }

    void to_json(json &j, const EntityManager &m)
    {
        j["living_entites"] = m.living_entites;
        j["living_entity_count"] = m.living_entity_count;
        for (Entity e : m.living_entites)
        {
            j["signatures"][std::to_string(e)] = m.signatures[e];
        }
    }

    World *load_world(const json &j, const ComponentRegistry &r)
    {
        World *world = new_world(r);
        component_manager_from_json(j["ComponentManager"], world->component_manager, engine_global()->component_registry);
        ComponentRegistry old_registry = j["ComponentRegistry"];
        entity_manager_from_json(j["EntityManager"], world->entity_manager, old_registry, engine_global()->component_registry);
        return world;
    }

    World *load_world(const VultrSource &file, const ComponentRegistry &r)
    {
        assert(file_exists(file) && "Couldn't find save file!");
        std::ifstream i;
        i.open(file.path);

        json world_saved_json;
        i >> world_saved_json;
        return load_world(world_saved_json, r);
    }

    void save_world(World *world, json &j)
    {
        json component_manager_json;
        component_manager_to_json(component_manager_json, world_get_component_manager(world), engine_global()->component_registry);
        json component_registry_json = engine_global()->component_registry;
        json entity_manager_json = world_get_entity_manager(world);
        j = {
            {"ComponentRegistry", component_registry_json},
            {"ComponentManager", component_manager_json},
            {"EntityManager", entity_manager_json},
        };
    }

    void save_world(World *_world, const VultrSource &out)
    {
        std::ofstream o;
        o.open(out.path);
        InternalWorld *world = static_cast<InternalWorld *>(_world);
        json final_output;
        save_world(world, final_output);
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
        component_manager_entity_destroyed(world->component_manager, entity);
        system_manager_entity_destroyed(world->system_manager, entity);
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
