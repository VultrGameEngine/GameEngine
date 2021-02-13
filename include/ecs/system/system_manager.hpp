// In charge of maintaining a record of registered systems and their signatures
// When a system is registered, it is added to a map
// If an entity that the system is tracking is destroyed, then it also needs to
// update its list
#pragma once
#include "../component/component.hpp"
#include "system.hpp"
#include <memory>
#include <unordered_map>

class SystemManager
{
  public:
    template <typename T> std::shared_ptr<T> RegisterSystem(Signature signature)
    {
        const char *type_name = typeid(T).name();

        assert(systems.find(type_name) == systems.end() &&
               "Registering system more than once");

        // Create a pointer to the system and return it so it can be used externally
        std::shared_ptr<T> system = std::make_shared<T>();
        systems.insert({type_name, system});

        SetSignature<T>(signature);

        return system;
    }

    template <typename T> void DeregisterSystem()
    {
        const char *type_name = typeid(T).name();

        assert(systems.find(type_name) != systems.end() &&
               "Attempting to deregister system that does not exist");

        // Remove the system from the map
        systems.erase(type_name);
    }

    template <typename T> void SetSignature(Signature signature)
    {
        const char *type_name = typeid(T).name();

        assert(systems.find(type_name) != systems.end() &&
               "System used before registered");

        // Set the signature for the system
        signatures.insert({type_name, signature});
    }

    void EntityDestroyed(Entity entity)
    {
        // Erase a destroyed entity from all system lists
        for (auto const &pair : systems)
        {
            auto const &system = pair.second;

            system->DestroyEntity(entity);
        }
    }

    void EntitySignatureChanged(Entity entity, Signature entity_signature)
    {
        // Notify each system that an entity's signature has changed
        for (auto const &pair : systems)
        {
            auto const &type = pair.first;
            auto const &system = pair.second;
            auto const &system_signature = signatures[type];

            // If entity signature matches system signature
            if (system->Match(entity_signature))
            {
                // Insert into set
                system->CreateEntity(entity);
            }
            // Entity signature does not match system signature
            else
            {
                // Erase from set
                system->DestroyEntity(entity);
            }
        }
    }

  private:
    // Map from system type string pointer to a signature
    std::unordered_map<const char *, Signature> signatures{};

    // Map from system type string pointer to a system pointer
    std::unordered_map<const char *, std::shared_ptr<System>> systems{};
};
