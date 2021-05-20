#include <ecs/component/component_manager.hpp>
#include <string.h>

namespace Vultr
{
    void entity_destroyed(ComponentManager &manager, Entity entity)
    {
        // Notify all component arrays that an entity has been destroyed
        for (auto const &pair : manager.component_arrays)
        {
            auto const &component = pair.second;
            component->EntityDestroyed(entity);
        }
    }

    // {
    //    "Components": {
    //        "ComponentName1": {
    //        	... list of component jsons ...
    //        },
    //        "ComponentName2": {
    //       	... list of component jsons ...
    //        },
    //
    //    }
    // }
    //
    //
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
            ComponentType type = r.component_name_to_type.at(component_name.c_str());
            if (m.component_arrays.find(type) != m.component_arrays.end())
            {
                m.component_arrays[type]->from_json(element.value());
            }
        }
    }

} // namespace Vultr
