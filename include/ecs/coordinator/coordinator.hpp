// Bundles the EntityManager, ComponentManager, and SystemManager into one coordinator
#pragma once
#include <memory>
#include "../component/component_manager.hpp"
#include "../component/component.hpp"
#include "../system/system_manager.hpp"
#include "../entity/entity_manager.hpp"
#include "../entity/entity.hpp"

class Coordinator
{
public:
    static Coordinator *Get();

    void Init()
    {
        component_manager = std::make_unique<ComponentManager>();
        entity_manager = std::make_unique<EntityManager>();
        system_manager = std::make_unique<SystemManager>();
    }

    // Entity methods
    Entity CreateEntity()
    {
        return entity_manager->CreateEntity();
    }

    void DestroyEntity(Entity entity)
    {
        entity_manager->DestroyEntity(entity);

        component_manager->EntityDestroyed(entity);

        // system_manager->EntityDestroyed(entity);
    }

    std::set<Entity> GetEntities(Signature signature)
    {
        return entity_manager->GetEntities(signature);
    }

    // Component methods
    template <typename T>
    void RegisterComponent()
    {
        component_manager->RegisterComponent<T>();
    }

    template <typename T>
    void AddComponent(Entity entity, T component)
    {
        component_manager->AddComponent<T>(entity, component);

        auto signature = entity_manager->GetSignature(entity);
        signature.set(component_manager->GetComponentType<T>(), true);
        entity_manager->SetSignature(entity, signature);

        // system_manager->EntitySignatureChanged(entity, signature);
    }

    template <typename T>
    void RemoveComponent(Entity entity)
    {
        component_manager->RemoveComponent<T>(entity);

        auto signature = entity_manager->GetSignature(entity);
        signature.set(component_manager->GetComponent<T>(), false);
        entity_manager->SetSignature(entity, signature);

        // system_manager->EntitySignatureChanged(entity, signature);
    }

    template <typename T>
    T &GetComponent(Entity entity)
    {
        return component_manager->GetComponent<T>(entity);
    }
    template <typename T>
    std::shared_ptr<ComponentArray<T>> GetComponents()
    {
        return component_manager->GetComponents<T>();
    }

    template <typename T>
    ComponentType GetComponentType()
    {
        return component_manager->GetComponentType<T>();
    }

    // System methods
    template <typename T>
    std::shared_ptr<T> RegisterSystem()
    {
        return system_manager->RegisterSystem<T>();
    }

    template <typename T>
    void DeregisterSystem()
    {
        return system_manager->DeregisterSystem<T>();
    }

    template <typename T>
    void SetSystemSignature(Signature signature)
    {
        // system_manager->SetSignature<T>(signature);
    }

private:
    static Coordinator *instance;

    std::unique_ptr<ComponentManager> component_manager;
    std::unique_ptr<EntityManager> entity_manager;
    std::unique_ptr<SystemManager> system_manager;
};
