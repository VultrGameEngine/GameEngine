#pragma once
#include <assert.h>
#include <ecs/component/component.hpp>
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
class Entity;
class SystemProvider
{
  public:
    // virtual const char *GetID() = 0;

    // Methods that are called by the world whenever an entity
    // is created or destroyed
    void DestroyEntity(Entity entity);

    void CreateEntity(Entity entity);

    virtual bool Match(Signature other);

    // A list of entities related to a specific System
    // automatically managed by this class and the world
    std::set<Entity> entities;
    Signature signature;

  protected:
    virtual void OnDestroyEntity(Entity entity) = 0;
    virtual void OnCreateEntity(Entity entity) = 0;
};
