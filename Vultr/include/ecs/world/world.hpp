// Bundles the EntityManager, ComponentManager, and SystemManager into one World
#pragma once
#define GLFW_INCLUDE_NONE
#include "../component/component.hpp"
#include "../component/component_manager.hpp"
#include "../entity/entity_manager.hpp"
#include "../system/system_manager.hpp"
#include <set>
#include "../../core/component_renderer.h"
#include <fstream>
#include <engine.hpp>

namespace Vultr
{

    struct World
    {

        static std::shared_ptr<World> Get();

        static void ChangeWorld(std::shared_ptr<World> world);

        static std::shared_ptr<World> Init();

        static void ExportWorld(const std::string &path,
                                std::shared_ptr<World> world);

        static void ExportWorldEditor(const std::string &path,
                                      std::shared_ptr<World> world);

        static std::shared_ptr<World> ImportWorld(const std::string &path);

        // Entity methods
        Entity CreateEntity();

        void DestroyEntity(Entity entity);

        std::set<Entity> GetEntities(Signature signature);

        Signature GetSignature(Entity entity);

        template <typename T>
        std::shared_ptr<ComponentArray<T>> GetComponents()
        {
            return component_manager_get_component_array<T>(component_manager);
        }

        // System methods
        template <typename T>
        std::shared_ptr<T> RegisterSystem(Signature signature)
        {
            return system_manager_register_system<T>(system_manager, signature);
        }

        template <typename T>
        void DeregisterSystem()
        {
            return system_manager_deregister_system<T>(system_manager);
        }

        template <typename T>
        std::shared_ptr<T> GetSystemProvider()
        {
            return system_manager_get_systemprovider<T>(system_manager);
        }

        ComponentManager component_manager;
        EntityManager entity_manager;
        SystemManager system_manager;

        static World *current_world;
    };

    template <typename T>
    void entity_add_component(Entity entity, T component)
    {
        std::shared_ptr<World> world = World::Get();
        assert(world != nullptr &&
               "World does not exist! Make sure you create a world "
               "before trying to add a component to an entity");
        component_manager_get_component_array<T>(world->component_manager)
            ->InsertData(entity, component);

        auto signature = entity_manager_get_signature(world->entity_manager, entity);
        signature.set(Vultr::Engine::GetComponentRegistry().GetComponentType<T>(),
                      true);
        system_manager_entity_signature_changed(world->system_manager, entity,
                                                signature);
        Vultr::Engine::GetGlobalSystemManager().EntitySignatureChanged(*this,
                                                                       signature);
        entity_manager_set_signature(world->entity_manager, entity, signature);
    }

    template <typename T>
    void entity_remove_component()
    {
        std::shared_ptr<World> world = World::Get();
        assert(world != nullptr &&
               "World does not exist! Make sure you create a world "
               "before trying to remove a component to an entity");
        auto signature = world->entity_manager->GetSignature(*this);
        signature.set(Vultr::Engine::GetComponentRegistry().GetComponentType<T>(),
                      false);
        world->system_manager->EntitySignatureChanged(*this, signature);
        Vultr::Engine::GetGlobalSystemManager().EntitySignatureChanged(*this,
                                                                       signature);
        world->entity_manager->SetSignature(*this, signature);

        world->component_manager->GetComponentArray<T>()->RemoveData(*this);
    }

    template <typename T>
    T &Entity::GetComponent()
    {
        std::shared_ptr<World> world = World::Get();
        assert(world != nullptr &&
               "Cannot get component because world does not exist!");
        return world->component_manager->GetComponentArray<T>()->GetData(*this);
    }

    template <typename T>
    bool Entity::HasComponent()
    {
        std::shared_ptr<World> world = World::Get();
        assert(world != nullptr &&
               "Cannot get component because world does not exist!");
        return world->component_manager->GetComponentArray<T>()->HasComponent(*this);
    }

    template <typename T>
    T *Entity::GetComponentUnsafe()
    {
        std::shared_ptr<World> world = World::Get();
        assert(world != nullptr &&
               "Cannot get component because world does not exist!");
        return world->component_manager->GetComponentArray<T>()->GetDataUnsafe(
            *this);
    }
} // namespace Vultr
