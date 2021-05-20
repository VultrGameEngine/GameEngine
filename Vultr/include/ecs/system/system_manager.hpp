// In charge of maintaining a record of registered systems and their signatures
// When a system is registered, it is added to a map
// If an entity that the system is tracking is destroyed, then it also needs to
// update its list
#pragma once
#include "../component/component.hpp"
#include "system_provider.hpp"
#include <unordered_map>
#include <iostream>
#include <type_info/type_info.h>

namespace Vultr
{

    // Holds a list of system components and signatures for those systems
    struct SystemManager
    {
        SystemManager() = default;
        // Map from system type string pointer to a system provider
        std::unordered_map<std::string, SystemProvider *> system_providers{};
    };

    template <typename T>
    T *system_manager_register_system(SystemManager &manager, Signature signature, OnCreateEntity on_create_entity, OnDestroyEntity on_destroy_entity, MatchSignature match_signature = nullptr)
    {
        static_assert(std::is_base_of<SystemProvider, T>::value &&
                      "System component is not a derived class of SystemProvider! Failed to register. Please make sure that the type provided is a subclass of SystemProvider");

        const char *type = get_struct_name<T>();

        assert(manager.system_providers.find(type) == manager.system_providers.end() && "Registering system provider more than once");

        // Create a pointer to the system and return it so it can be used externally
        auto *instance = new T();
        auto *system_provider = static_cast<SystemProvider *>(instance);
        system_provider->signature = signature;
        system_provider->on_create_entity = on_create_entity;
        system_provider->on_destroy_entity = on_destroy_entity;
        system_provider->match_signature = match_signature;
        manager.system_providers.insert({type, system_provider});

        return instance;
    }

    template <typename T>
    void system_manager_deregister_system(SystemManager &manager)
    {
        static_assert(std::is_base_of<SystemProvider, T>::value &&
                      "System component is not a derived class of SystemProvider! Failed to deregister. Please make sure that the type provided is a subclass of SystemProvider");

        const char *type = get_struct_name<T>();
        assert(manager.system_providers.find(type) != manager.system_providers.end() && "Attempting to deregister system that does not exist");

        delete manager.system_providers[type];

        // Remove the system from the map
        manager.system_providers.erase(type);
    }

    template <typename T>
    T *system_manager_get_system_provider(SystemManager &manager)
    {
        static_assert(std::is_base_of<SystemProvider, T>::value &&
                      "System component is not a derived class of SystemProvider! Failed to get. Please make sure that the type provided is a subclass of SystemProvider");

        const char *type = get_struct_name<T>();
        assert(manager.system_providers.find(type) != manager.system_providers.end() && "System used before registered");

        return dynamic_cast<T *>(manager.system_providers.at(type));
    }

    // Called by world or engine to update the systems when new entities are created
    void system_manager_entity_destroyed(SystemManager &manager, Entity entity);

    void system_manager_entity_signature_changed(SystemManager &manager, Entity entity, Signature entity_signature);
} // namespace Vultr
