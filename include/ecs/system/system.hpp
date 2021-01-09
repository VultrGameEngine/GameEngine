// A System is functionality that iterates upon a list of entities with a
// certain signature of components This System class is to be inherited by real
// system classes

#pragma once
#include <cassert>
#include <ecs/component/component.hpp>
#include <ecs/component/component_array.hpp>
#include <ecs/entity/entity.hpp>
#include <ecs/system/system_provider.hpp>
#include <memory>
#include <set>

class System
{
  public:
    // Methods that are called by the world whenever an entity
    // is created or destroyed
    void DestroyEntity(Entity entity)
    {
        SystemProvider &provider = GetProvider();
        if (provider.entities.find(entity) == provider.entities.end())
            return;
        provider.entities.erase(entity);
        OnDestroyEntity(entity);
    }

    void CreateEntity(Entity entity)
    {
        GetProvider().entities.insert(entity);
        OnCreateEntity(entity);
    }

  protected:
    // Methods that can be overriden by the child classes in order
    // perform special methods when a new entity is created or destroyed
    virtual void OnDestroyEntity(Entity entity)
    {
    }

    virtual void OnCreateEntity(Entity entity)
    {
    }

    // Get the system provider for this system which will hold the
    // current entities of the system
    // MUST BE IMPLEMENTED
    virtual SystemProvider &GetProvider() = 0;
};
