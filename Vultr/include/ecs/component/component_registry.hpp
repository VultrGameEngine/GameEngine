#pragma once
#include "component.hpp"
#include "component_array.hpp"
#include <unordered_map>
#include "../../core/component_renderer.h"
#include "../../core/component_constructor.h"

#define CallConstructor(entity, T) entity.AddComponent(T::Create())
#define CallRenderer(entity, T) RenderComponent<T>(entity)

namespace Vultr
{
class ComponentRegistry
{
  public:
    template <typename T> void RegisterComponent(bool inspector_available = true)
    {
        const char *type_name = typeid(T).name();

        assert(component_types.find(type_name) == component_types.end() &&
               "Registered component type more than once");

        // Add the component type to the map
        component_types.insert({type_name, next_component_type});

        ComponentRender renderer = [](Entity entity) { CallRenderer(entity, T); };

        // Add the component to the map
        component_renderers.insert({type_name, renderer});

        ComponentConstructor constructor = [](Entity entity) {
            CallConstructor(entity, T);
        };

        component_constructors.insert({type_name, constructor});

        // Increment the next component type
        ++next_component_type;
    }

    template <typename T> void RegisterMaterial()
    {
        const char *type_name = typeid(T).name();

        ComponentRender renderer = [](Entity entity) { CallRenderer(entity, T); };

        // Add the component to the map
        component_renderers.insert({type_name, renderer});

        ComponentConstructor constructor = [](Entity entity) {
            CallConstructor(entity, T);
        };

        component_constructors.insert({type_name, constructor});
    }

    template <typename T> ComponentType GetComponentType()
    {
        const char *type_name = typeid(T).name();

        assert(component_types.find(type_name) != component_types.end() &&
               "Component not registered before use");

        // Return this component's type used for creating signatures
        return component_types[type_name];
    }

    bool ComponentRegistered(const char *type_name)
    {
        return component_types.find(type_name) != component_types.end();
    }

    void RenderEntityComponents(Entity entity)
    {
        for (auto const [type, render] : component_renderers)
        {
            render(entity);
        }
    }
    std::unordered_map<const char *, ComponentConstructor> GetComponentConstructors()
    {
        return component_constructors;
    }

  private:
    // Map from type string pointer to a component type
    std::unordered_map<const char *, ComponentType> component_types{};

    // Map from type string pointer to a render function
    std::unordered_map<const char *, ComponentRender> component_renderers{};

    // Map from name to the creator, for use in the editor
    std::unordered_map<const char *, ComponentConstructor> component_constructors{};

    // The component type to be assigned to the next registered component starting at
    // 0
    ComponentType next_component_type{};
};
} // namespace Vultr
