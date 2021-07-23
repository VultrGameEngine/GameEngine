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

    void component_registry_render_entity_components(Engine *e, const ComponentRegistry &r, Entity entity)
    {
        for (auto const [type, data] : r.components)
        {
            data.component_renderer(e, entity);
        }
    }

    void component_registry_delete_game_components(ComponentRegistry &r)
    {
        for (auto type_name : r.game_components)
        {
            auto type = r.component_name_to_type[type_name];
            r.component_name_to_type.erase(type_name);
            r.component_type_to_name.erase(type);
            r.components.erase(type);
        }
        r.game_components.clear();
    }

} // namespace Vultr
