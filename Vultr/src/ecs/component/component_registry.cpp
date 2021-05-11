#include <ecs/component/component_registry.hpp>

namespace Vultr
{

    bool component_registry_is_component_registered(const ComponentRegistry &r,
                                                    ComponentType type)
    {
        return r.components.find(type) != r.components.end();
    }

    void component_regsitry_render_entity_components(const ComponentRegistry &r,
                                                     Entity entity)
    {
        for (auto const [type, data] : r.components)
        {
            data.component_renderer(entity);
        }
    }
} // namespace Vultr
