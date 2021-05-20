#include <ecs/component/component_registry.hpp>

namespace Vultr
{
    const char *component_registry_get_component_name(ComponentRegistry &r, ComponentType type)
    {
        assert(component_registry_is_component_registered(r, type) && "Component not registered before use!");
        return r.component_type_to_name[type].c_str();
    }

    bool component_registry_is_component_registered(const ComponentRegistry &r, ComponentType type)
    {
        return r.components.find(type) != r.components.end();
    }

    void component_registry_render_entity_components(const ComponentRegistry &r, Entity entity)
    {
        for (auto const [type, data] : r.components)
        {
            data.component_renderer(entity);
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
} // namespace Vultr
