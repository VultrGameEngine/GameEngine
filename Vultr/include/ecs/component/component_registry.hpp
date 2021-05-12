#pragma once
#include "component.hpp"
#include "component_array.hpp"
#include <unordered_map>
#include "../../core/component_renderer.h"
#include "../../core/component_constructor.h"
#include <type_info/type_info.h>

namespace Vultr
{
    struct ComponentRegistry
    {
        struct ComponentData
        {
            ComponentRender component_renderer;
            ComponentConstructor component_constructor;
        };

        // Map from component type to a component data
        std::unordered_map<ComponentType, ComponentData> components{};
    };

    template <typename T>
    void component_registry_register_component(ComponentRegistry &r)
    {
        ComponentType type = hash_struct<T>();

        assert(r.components.find(type) == r.components.end() &&
               "Registered component type more than once");

        // Create the renderer
        ComponentRender renderer = [](Entity entity) { RenderComponent<T>(entity); };

        ComponentConstructor constructor = [](Entity entity) {
            entity_add_component(entity, T::Create());
        };

        // Create the tuple with the renderer and constructor
        ComponentRegistry::ComponentData data = {
            .component_renderer = renderer, .component_constructor = constructor};

        // Add the component type to the map
        r.components.insert({type, data});
    }

    bool component_registry_is_component_registered(const ComponentRegistry &r,
                                                    ComponentType type);

    void component_regsitry_render_entity_components(const ComponentRegistry &r,
                                                     Entity entity);
} // namespace Vultr
