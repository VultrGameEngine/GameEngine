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
#include <cereal/archives/binary.hpp>
#include <cereal/types/polymorphic.hpp>
#include <fstream>
#include <engine.hpp>

class World
{
  public:
    World() = default;
    template <class Archive> void serialize(Archive &ar)
    {
        ar(component_manager, entity_manager, system_manager);
    }

    static std::shared_ptr<World> Get()
    {
        return current_world;
    }
    static void ChangeWorld(std::shared_ptr<World> world)
    {
        current_world = world;
        // world->system_manager = std::make_unique<SystemManager>();
    }

    static std::shared_ptr<World> Init()
    {
        std::shared_ptr<World> world = std::make_shared<World>();
        world->component_manager = std::make_unique<ComponentManager>();
        world->entity_manager = std::make_unique<EntityManager>();
        world->system_manager = std::make_unique<SystemManager>();
        return world;
    }

    static void ExportWorld(const std::string &path, std::shared_ptr<World> world)
    {
        std::ofstream os(path);
        cereal::BinaryOutputArchive oarchive(os);

        // oarchive(world);
    }

    static std::shared_ptr<World> ImportWorld(const std::string &path)
    {
        std::shared_ptr<World> world;
        std::ifstream is(path);
        cereal::BinaryInputArchive iarchive(is);

        // iarchive(world);
        return world;
    }

    // Entity methods
    static Entity CreateEntity()
    {
        return Get()->entity_manager->CreateEntity();
    }

    static void DestroyEntity(Entity entity)
    {
        Get()->entity_manager->DestroyEntity(entity);

        Get()->system_manager->EntityDestroyed(entity);

        Vultr::Engine::GetGlobalSystemManager().EntityDestroyed(entity);

        Get()->component_manager->EntityDestroyed(entity);
    }

    static std::set<Entity> GetEntities(Signature signature)
    {
        return Get()->entity_manager->GetEntities(signature);
    }

    static Signature GetSignature(Entity entity)
    {
        return Get()->entity_manager->GetSignature(entity);
    }

    template <typename T> static void AddComponent(Entity entity, T component)
    {
        Get()->component_manager->AddComponent<T>(entity, component);

        auto signature = Get()->entity_manager->GetSignature(entity);
        signature.set(Vultr::Engine::GetComponentRegistry().GetComponentType<T>(),
                      true);
        Get()->system_manager->EntitySignatureChanged(entity, signature);
        Vultr::Engine::GetGlobalSystemManager().EntitySignatureChanged(entity,
                                                                       signature);
        Get()->entity_manager->SetSignature(entity, signature);
    }

    template <typename T> static void RemoveComponent(Entity entity)
    {

        auto signature = Get()->entity_manager->GetSignature(entity);
        signature.set(Vultr::Engine::GetComponentRegistry().GetComponentType<T>(),
                      false);
        Get()->system_manager->EntitySignatureChanged(entity, signature);
        Vultr::Engine::GetGlobalSystemManager().EntitySignatureChanged(entity,
                                                                       signature);
        Get()->entity_manager->SetSignature(entity, signature);

        Get()->component_manager->RemoveComponent<T>(entity);
    }

    template <typename T> static T &GetComponent(Entity entity)
    {
        return Get()->component_manager->GetComponent<T>(entity);
    }

    template <typename T> static T *GetComponentUnsafe(Entity entity)
    {
        return Get()->component_manager->GetComponentUnsafe<T>(entity);
    }

    template <typename T> static std::shared_ptr<ComponentArray<T>> GetComponents()
    {
        return Get()->component_manager->GetComponents<T>();
    }

    // System methods
    template <typename T>
    static std::shared_ptr<T> RegisterSystem(Signature signature)
    {
        return Get()->system_manager->RegisterSystem<T>(signature);
    }

    template <typename T> static void SetSignature(Signature signature)
    {
        return Get()->system_manager->SetSignature<T>(signature);
    }

    template <typename T> static void DeregisterSystem()
    {
        return Get()->system_manager->DeregisterSystem<T>();
    }

    template <typename T> static std::shared_ptr<T> GetSystemProvider()
    {
        return Get()->system_manager->GetSystemProvider<T>();
    }

    std::unique_ptr<ComponentManager> component_manager;
    std::unique_ptr<EntityManager> entity_manager;
    std::unique_ptr<SystemManager> system_manager;

  private:
    static std::shared_ptr<World> current_world;
};

template <typename T> void Entity::AddComponent(T component)
{
    World::AddComponent(*this, component);
}

template <typename T> void Entity::RemoveComponent()
{
    World::RemoveComponent<T>(*this);
}

template <typename T> T &Entity::GetComponent()
{
    return World::GetComponent<T>(*this);
}

template <typename T> T *Entity::GetComponentUnsafe()
{
    return World::GetComponentUnsafe<T>(*this);
}
