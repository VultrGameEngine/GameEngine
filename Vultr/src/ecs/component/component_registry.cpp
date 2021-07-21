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

} // namespace Vultr
