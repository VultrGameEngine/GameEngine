// Bundles the EntityManager, ComponentManager, and SystemManager into one World
#pragma once
#include <memory>
#include "../component/component_manager.hpp"
#include "../component/component.hpp"
#include "../system/system_manager.hpp"
#include "../entity/entity_manager.hpp"
#include "../entity/entity.hpp"

class World
{
public:
    static void Init()
    {
        Get()->component_manager = std::make_unique<ComponentManager>();
        Get()->entity_manager = std::make_unique<EntityManager>();
        Get()->system_manager = std::make_unique<SystemManager>();
    }

    // Entity methods
    static Entity CreateEntity()
    {
        return Get()->entity_manager->CreateEntity();
    }

    static void DestroyEntity(Entity entity)
    {
        Get()->entity_manager->DestroyEntity(entity);

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

    // Component methods
    template <typename T>
    static void RegisterComponent()
    {
        Get()->component_manager->RegisterComponent<T>();
    }

    template <typename T>
    static void AddComponent(Entity entity, T component)
    {
        Get()->component_manager->AddComponent<T>(entity, component);

        auto signature = Get()->entity_manager->GetSignature(entity);
        signature.set(Get()->component_manager->GetComponentType<T>(), true);
        Get()->entity_manager->SetSignature(entity, signature);
    }

    template <typename T>
    static void RemoveComponent(Entity entity)
    {
        Get()->component_manager->RemoveComponent<T>(entity);

        auto signature = Get()->entity_manager->GetSignature(entity);
        signature.set(Get()->component_manager->GetComponent<T>(), false);
        Get()->entity_manager->SetSignature(entity, signature);
    }

    template <typename T>
    static T &GetComponent(Entity entity)
    {
        return Get()->component_manager->GetComponent<T>(entity);
    }
    template <typename T>
    static std::shared_ptr<ComponentArray<T>> GetComponents()
    {
        return Get()->component_manager->GetComponents<T>();
    }

    template <typename T>
    static ComponentType GetComponentType()
    {
        return Get()->component_manager->GetComponentType<T>();
    }

    // System methods
    template <typename T>
    static std::shared_ptr<T> RegisterSystem()
    {
        return Get()->system_manager->RegisterSystem<T>();
    }

    template <typename T>
    static void DeregisterSystem()
    {
        return Get()->system_manager->DeregisterSystem<T>();
    }

private:
    static World *Get();
    std::unique_ptr<ComponentManager> component_manager;
    std::unique_ptr<EntityManager> entity_manager;
    std::unique_ptr<SystemManager> system_manager;
};
