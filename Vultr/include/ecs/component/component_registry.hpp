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

        // Map from component stringified name to a component type
        std::unordered_map<const char *, ComponentType> component_name_to_type{};

        // Map from component type to component stringified name
        std::unordered_map<ComponentType, const char *> component_type_to_name{};

        // The component type to be assigned to the next registered component
        // starting at
        // 0
        ComponentType next_component_type{};
    };

    template <typename T>
    void component_registry_register_component(ComponentRegistry &r, ComponentConstructor constructor)
    {
        const char *type_name = get_struct_name<T>();

        assert(r.component_name_to_type.find(type_name) == r.component_name_to_type.end() && "Registered component type more than once");

        ComponentType type = r.next_component_type;
        r.component_name_to_type[type_name] = type;
        r.component_type_to_name[type] = type_name;

        // Create the renderer
        ComponentRender renderer = [](Entity entity) { RenderComponent<T>(entity); };

        // Create the tuple with the renderer and constructor
        ComponentRegistry::ComponentData data = {.component_renderer = renderer, .component_constructor = constructor};

        // Add the component type to the map
        r.components.insert({type, data});
    }

    template <typename T>
    ComponentType component_registry_get_component_type(ComponentRegistry &r)
    {
        const char *type_name = get_struct_name<T>();
        assert(r.component_name_to_type.find(type_name) != r.component_name_to_type.end() && "Component not registered before use");

        // Return this component's type used for creating signatures
        return r.component_name_to_type[type_name];
    }

    const char *component_registry_get_component_name(ComponentRegistry &r, ComponentType type);

    bool component_registry_is_component_registered(const ComponentRegistry &r, ComponentType type);

    void component_registry_render_entity_components(const ComponentRegistry &r, Entity entity);
} // namespace Vultr
