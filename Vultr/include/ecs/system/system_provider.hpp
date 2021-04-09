#pragma once
#include <assert.h>
#include <cereal/types/bitset.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/set.hpp>
#include <ecs/component/component.hpp>
#include <ecs/entity/entity.hpp>
#include <set>

template <typename T> std::string GetName()
{
    assert(true && "Please call VultrRegisterSystemProvider to create the template "
                   "specialization for this function");
}

#define VultrRegisterSystemProvider(T)                                              \
    template <> inline std::string GetName<T>()                                     \
    {                                                                               \
        return #T;                                                                  \
    }

class SystemProvider
{
  public:
    // virtual const char *GetID() = 0;

    // Methods that are called by the world whenever an entity
    // is created or destroyed
    void DestroyEntity(Entity entity)
    {
        if (entities.find(entity) == entities.end())
            return;
        entities.erase(entity);
        OnDestroyEntity(entity);
    }

    void CreateEntity(Entity entity)
    {
        entities.insert(entity);
        OnCreateEntity(entity);
    }

    virtual bool Match(Signature other)
    {
        return (other & signature) == signature;
    }

    template <class Archive> void serialize(Archive &ar)
    {
        ar(signature, entities);
    }
    // A list of entities related to a specific System
    // automatically managed by this class and the world
    std::set<Entity> entities;
    Signature signature;

  protected:
    virtual void OnDestroyEntity(Entity entity) = 0;
    virtual void OnCreateEntity(Entity entity) = 0;
};
