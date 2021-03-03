// In charge of maintaining a record of registered systems and their signatures
// When a system is registered, it is added to a map
// If an entity that the system is tracking is destroyed, then it also needs to
// update its list
#pragma once
#include "../component/component.hpp"
#include <cereal/types/bitset.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/unordered_map.hpp>
#include "../system/system_provider.hpp"
#include <memory>
#include <unordered_map>

class SystemManager
{
  public:
    template <class Archive> void serialize(Archive &ar)
    {
        ar(signatures, system_providers);
    }

    template <typename T> std::shared_ptr<T> RegisterSystem(Signature signature)
    {
        const char *type_name = typeid(T).name();

        assert(system_providers.find(type_name) == system_providers.end() &&
               "Registering system provider more than once");

        // Create a pointer to the system and return it so it can be used externally
        std::shared_ptr<T> system_provider = std::make_shared<T>();
        ((std::shared_ptr<SystemProvider>)system_provider)->signature = signature;
        system_providers.insert({type_name, system_provider});

        SetSignature<T>(signature);

        return system_provider;
    }

    template <typename T> void DeregisterSystem()
    {
        const char *type_name = typeid(T).name();

        assert(system_providers.find(type_name) != system_providers.end() &&
               "Attempting to deregister system that does not exist");

        // Remove the system from the map
        system_providers.erase(type_name);
    }

    template <typename T> void SetSignature(Signature signature)
    {
        const char *type_name = typeid(T).name();

        assert(system_providers.find(type_name) != system_providers.end() &&
               "System used before registered");

        // Set the signature for the system
        signatures.insert({type_name, signature});
    }

    template <typename T> std::shared_ptr<T> GetSystemProvider()
    {
        const char *type_name = typeid(T).name();

        assert(system_providers.find(type_name) != system_providers.end() &&
               "System used before registered");

        return std::dynamic_pointer_cast<T>(system_providers.at(type_name));
    }

    void EntityDestroyed(Entity entity)
    {
        // Erase a destroyed entity from all system lists
        for (auto const &pair : system_providers)
        {
            auto const &system = pair.second;

            system->DestroyEntity(entity);
        }
    }

    void EntitySignatureChanged(Entity entity, Signature entity_signature)
    {
        // Notify each system that an entity's signature has changed
        for (auto const &pair : system_providers)
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

    // Map from system type string pointer to a system provider
    std::unordered_map<const char *, std::shared_ptr<SystemProvider>>
        system_providers{};
};
