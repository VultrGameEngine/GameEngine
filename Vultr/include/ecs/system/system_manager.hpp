// In charge of maintaining a record of registered systems and their signatures
// When a system is registered, it is added to a map
// If an entity that the system is tracking is destroyed, then it also needs to
// update its list
#pragma once
#include "../component/component.hpp"
#include "system_provider.hpp"
#include <unordered_map>

namespace Vultr
{

    // Holds a list of system components and signatures for those systems
    struct SystemManager
    {
        // Map from system type string pointer to a system provider
        std::unordered_map<const char *, SystemProvider> system_providers{};
    };

    template <typename T>
    std::shared_ptr<T> system_manager_register_system(SystemManager &manager,
                                                      Signature signature)
    {
        const char *type_name = system_provider_get_name<T>();

        assert(manager.system_providers.find(type_name) ==
                   manager.system_providers.end() &&
               "Registering system provider more than once");

        // Create a pointer to the system and return it so it can be used externally
        std::shared_ptr<T> system_provider = std::make_shared<T>();
        ((std::shared_ptr<SystemProvider>)system_provider)->signature = signature;
        manager.system_providers.insert({type_name, system_provider});

        return system_provider;
    }

    template <typename T>
    void system_manager_deregister_system(SystemManager &manager)
    {
        const char *type_name = system_provider_get_name<T>();

        assert(manager.system_providers.find(type_name) !=
                   manager.system_providers.end() &&
               "Attempting to deregister system that does not exist");

        // Remove the system from the map
        manager.system_providers.erase(type_name);
    }

    template <typename T>
    std::shared_ptr<T> system_manager_get_systemprovider(SystemManager &manager)
    {
        const char *type_name = system_provider_get_name<T>();

        assert(manager.system_providers.find(type_name) !=
                   manager.system_providers.end() &&
               "System used before registered");

        return std::dynamic_pointer_cast<T>(manager.system_providers.at(type_name));
    }

    // Called by world or engine to update the systems when new entities are created
    void system_manager_entity_destroyed(SystemManager &manager, Entity entity);

    void system_manager_entity_signature_changed(SystemManager &manager,
                                                 Entity entity,
                                                 Signature entity_signature);
} // namespace Vultr
