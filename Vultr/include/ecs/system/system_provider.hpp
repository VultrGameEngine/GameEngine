#pragma once
#include <assert.h>
#include <ecs/entity/entity.hpp>
#include <ecs/component/component.hpp>
#include <set>

namespace Vultr
{
    typedef void (*OnCreateEntity)(Entity);
    typedef void (*OnDestroyEntity)(Entity);
    typedef bool (*MatchSignature)(Signature other);

    // Alias to describe the ID of the systems
    // Every system has an ID which is just a number so we define [SystemType]
    // as just a uint32_t
    //
    // This will be set at compile time
    typedef u32 SystemType;

    struct SystemProvider
    {
        virtual ~SystemProvider()
        {
        }
        // A list of entities related to a specific System
        // automatically managed by this class and the world
        std::set<Entity> entities;
        Signature signature;

        // Optional callbacks that can be set by systems to customize functionality
        // on creation and destruction of entities
        //
        // Will be called by SystemManager
        OnCreateEntity on_create_entity = nullptr;
        OnDestroyEntity on_destroy_entity = nullptr;

        // Optional callback to define custom signature matching for system. If this
        // is provided then when receiving a signature, you will return a boolean
        // based on your matching method
        //
        // By default this is simply the signature_contains function
        //
        // See include/ecs/component/component.hpp
        MatchSignature match_signature = nullptr;
    };

    void system_provider_on_create_entity(SystemProvider &provider, Entity entity);
    void system_provider_on_destroy_entity(SystemProvider &provider, Entity entity);

} // namespace Vultr
