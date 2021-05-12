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
        ComponentManager component_manager;
        EntityManager entity_manager;
        SystemManager system_manager;
    };

    void ChangeWorld(const World &world);

    World Init();

    void ExportWorld(const std::string &path, std::shared_ptr<World> world);

    void ExportWorldEditor(const std::string &path, std::shared_ptr<World> world);

    World ImportWorld(const std::string &path);

    // Entity methods
    Entity create_entity(World &world);

    void DestroyEntity(Entity entity);

    std::set<Entity> GetEntities(Signature signature);

    Signature GetSignature(Entity entity);

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
        signature.set(Engine::GetComponentRegistry().GetComponentType<T>(), true);
        system_manager_entity_signature_changed(world->system_manager, entity,
                                                signature);
        Engine::GetGlobalSystemManager().EntitySignatureChanged(*this, signature);
        entity_manager_set_signature(world->entity_manager, entity, signature);
    }

    template <typename T>
    void entity_remove_component(Entity entity)
    {
        std::shared_ptr<World> world = World::Get();
        assert(world != nullptr &&
               "World does not exist! Make sure you create a world "
               "before trying to remove a component to an entity");
        auto signature = world->entity_manager->GetSignature(*this);
        signature.set(Engine::GetComponentRegistry().GetComponentType<T>(), false);
        world->system_manager->EntitySignatureChanged(*this, signature);
        Engine::GetGlobalSystemManager().EntitySignatureChanged(*this, signature);
        entity_manager_set_signature(world->entity_manager, );
        world->entity_manager->SetSignature(*this, signature);

        world->component_manager->GetComponentArray<T>()->RemoveData(*this);
    }

    template <typename T>
    T &entity_get_component(Entity entity)
    {
        std::shared_ptr<World> world = World::Get();
        assert(world != nullptr &&
               "Cannot get component because world does not exist!");
        return world->component_manager->GetComponentArray<T>()->GetData(*this);
    }

    template <typename T>
    bool entity_has_component(Entity entity)
    {
        std::shared_ptr<World> world = World::Get();
        assert(world != nullptr &&
               "Cannot get component because world does not exist!");
        return world->component_manager->GetComponentArray<T>()->HasComponent(
            entity);
    }

    template <typename T>
    T *entity_get_component_unsafe(Entity entity)
    {
        std::shared_ptr<World> world = World::Get();
        assert(world != nullptr &&
               "Cannot get component because world does not exist!");
        return world->component_manager->GetComponentArray<T>()->GetDataUnsafe(
            entity);
    }
} // namespace Vultr
