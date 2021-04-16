// Bundles the EntityManager, ComponentManager, and SystemManager into one World
#pragma once
#define GLFW_INCLUDE_NONE
#include "../component/component.hpp"
#include "../component/component_manager.hpp"
#include "../entity/entity_manager.hpp"
#include "../system/system_manager.hpp"
#include <memory>
#include <set>
#include "../../core/component_renderer.h"
#include <fstream>
#include <engine.hpp>

class World
{
  public:
    World() = default;

    static std::shared_ptr<World> Get();

    static void ChangeWorld(std::shared_ptr<World> world);

    static std::shared_ptr<World> Init();

    static void ExportWorld(const std::string &path, std::shared_ptr<World> world);

    static void ExportWorldEditor(const std::string &path,
                                  std::shared_ptr<World> world);

    static std::shared_ptr<World> ImportWorld(const std::string &path);

    // Entity methods
    Entity CreateEntity();

    void DestroyEntity(Entity entity);

    std::set<Entity> GetEntities(Signature signature);

    Signature GetSignature(Entity entity);

    template <typename T> std::shared_ptr<ComponentArray<T>> GetComponents()
    {
        return component_manager->GetComponentArray<T>();
    }

    // System methods
    template <typename T> std::shared_ptr<T> RegisterSystem(Signature signature)
    {
        return system_manager->RegisterSystem<T>(signature);
    }

    template <typename T> void SetSignature(Signature signature)
    {
        return system_manager->SetSignature<T>(signature);
    }

    template <typename T> void DeregisterSystem()
    {
        return system_manager->DeregisterSystem<T>();
    }

    template <typename T> std::shared_ptr<T> GetSystemProvider()
    {
        return system_manager->GetSystemProvider<T>();
    }

    std::unique_ptr<ComponentManager> component_manager;
    std::unique_ptr<EntityManager> entity_manager;
    std::unique_ptr<SystemManager> system_manager;

  private:
    static std::shared_ptr<World> current_world;
    friend Vultr::Engine;
    friend Entity;
};

template <typename T> void Entity::AddComponent(T component)
{
    std::shared_ptr<World> world = World::Get();
    assert(world != nullptr && "World does not exist! Make sure you create a world "
                               "before trying to add a component to an entity");
    world->component_manager->GetComponentArray<T>()->InsertData(*this, component);

    auto signature = world->entity_manager->GetSignature(*this);
    signature.set(Vultr::Engine::GetComponentRegistry().GetComponentType<T>(), true);
    world->system_manager->EntitySignatureChanged(*this, signature);
    Vultr::Engine::GetGlobalSystemManager().EntitySignatureChanged(*this, signature);
    world->entity_manager->SetSignature(*this, signature);
}

template <typename T> void Entity::RemoveComponent()
{
    std::shared_ptr<World> world = World::Get();
    assert(world != nullptr && "World does not exist! Make sure you create a world "
                               "before trying to remove a component to an entity");
    auto signature = world->entity_manager->GetSignature(*this);
    signature.set(Vultr::Engine::GetComponentRegistry().GetComponentType<T>(),
                  false);
    world->system_manager->EntitySignatureChanged(*this, signature);
    Vultr::Engine::GetGlobalSystemManager().EntitySignatureChanged(*this, signature);
    world->entity_manager->SetSignature(*this, signature);

    world->component_manager->GetComponentArray<T>()->RemoveData(*this);
}

template <typename T> T &Entity::GetComponent()
{
    std::shared_ptr<World> world = World::Get();
    assert(world != nullptr && "Cannot get component because world does not exist!");
    return world->component_manager->GetComponentArray<T>()->GetData(*this);
}

template <typename T> bool Entity::HasComponent()
{
    std::shared_ptr<World> world = World::Get();
    assert(world != nullptr && "Cannot get component because world does not exist!");
    return world->component_manager->GetComponentArray<T>()->HasComponent(*this);
}

template <typename T> T *Entity::GetComponentUnsafe()
{
    std::shared_ptr<World> world = World::Get();
    assert(world != nullptr && "Cannot get component because world does not exist!");
    return world->component_manager->GetComponentArray<T>()->GetDataUnsafe(*this);
}
