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
        if (current_world != nullptr)
        {
            for (EntityID id : current_world->entity_manager->living_entites)
            {
                Entity entity(id);
                Vultr::Engine::GetGlobalSystemManager().EntityDestroyed(entity);
            }
        }

        current_world = world;
        for (EntityID id : current_world->entity_manager->living_entites)
        {
            Entity entity(id);
            Signature signature =
                current_world->entity_manager->GetSignature(entity);
            Vultr::Engine::GetGlobalSystemManager().EntitySignatureChanged(
                id, signature);
        }
        world->system_manager = std::make_unique<SystemManager>();
    }

    std::shared_ptr<World> Init()
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

        oarchive(world);
    }

    static std::shared_ptr<World> ImportWorld(const std::string &path)
    {
        std::shared_ptr<World> world;
        std::ifstream is(path);
        cereal::BinaryInputArchive iarchive(is);

        iarchive(world);
        return world;
    }

    // Entity methods
    Entity CreateEntity()
    {
        return entity_manager->CreateEntity();
    }

    void DestroyEntity(Entity entity)
    {
        entity_manager->DestroyEntity(entity);

        system_manager->EntityDestroyed(entity);

        Vultr::Engine::GetGlobalSystemManager().EntityDestroyed(entity);

        component_manager->EntityDestroyed(entity);
    }

    std::set<Entity> GetEntities(Signature signature)
    {
        return entity_manager->GetEntities(signature);
    }

    Signature GetSignature(Entity entity)
    {
        return entity_manager->GetSignature(entity);
    }

    template <typename T> void AddComponent(Entity entity, T component)
    {
        component_manager->AddComponent<T>(entity, component);

        auto signature = entity_manager->GetSignature(entity);
        signature.set(Vultr::Engine::GetComponentRegistry().GetComponentType<T>(),
                      true);
        system_manager->EntitySignatureChanged(entity, signature);
        Vultr::Engine::GetGlobalSystemManager().EntitySignatureChanged(entity,
                                                                       signature);
        entity_manager->SetSignature(entity, signature);
    }

    template <typename T> void RemoveComponent(Entity entity)
    {

        auto signature = entity_manager->GetSignature(entity);
        signature.set(Vultr::Engine::GetComponentRegistry().GetComponentType<T>(),
                      false);
        system_manager->EntitySignatureChanged(entity, signature);
        Vultr::Engine::GetGlobalSystemManager().EntitySignatureChanged(entity,
                                                                       signature);
        entity_manager->SetSignature(entity, signature);

        component_manager->RemoveComponent<T>(entity);
    }

    template <typename T> T &GetComponent(Entity entity)
    {
        return component_manager->GetComponent<T>(entity);
    }

    template <typename T> T *GetComponentUnsafe(Entity entity)
    {
        return component_manager->GetComponentUnsafe<T>(entity);
    }

    template <typename T> std::shared_ptr<ComponentArray<T>> GetComponents()
    {
        return component_manager->GetComponents<T>();
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
};

template <typename T> void Entity::AddComponent(T component)
{
    World::Get()->AddComponent(*this, component);
}

template <typename T> void Entity::RemoveComponent()
{
    World::Get()->RemoveComponent<T>(*this);
}

template <typename T> T &Entity::GetComponent()
{
    return World::Get()->GetComponent<T>(*this);
}

template <typename T> T *Entity::GetComponentUnsafe()
{
    return World::Get()->GetComponentUnsafe<T>(*this);
}
