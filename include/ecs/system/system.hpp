// A System is functionality that iterates upon a list of entities with a
// certain signature of components This System class is to be inherited by real
// system classes

#pragma once
#include "../component/component.hpp"
#include "../component/component_array.hpp"
#include "../entity/entity.hpp"
#include <cassert>
#include <memory>
#include <set>

class System {
public:
  // Methods that are called by the world whenever an entity
  // is created or destroyed
  void DestroyEntity(Entity entity) {
    if (entities.find(entity) == entities.end())
      return;
    entities.erase(entity);
    OnDestroyEntity(entity);
  }

  void CreateEntity(Entity entity) {
    entities.insert(entity);
    OnCreateEntity(entity);
  }

  // Methods that can be overriden by the child classes in order
  // perform special methods when a new entity is created or destroyed
  virtual void OnDestroyEntity(Entity entity) {}

  virtual void OnCreateEntity(Entity entity) {}

  // A list of entities related to a specific System
  // automatically managed by this class and the world
  std::set<Entity> entities;
};
