#pragma once
#include <ecs/component/component.hpp>
#include <ecs/entity/entity.hpp>
#include <set>

class SystemProvider
{
  public:
    // A list of entities related to a specific System
    // automatically managed by this class and the world
    std::set<Entity> entities;
    Signature signature;
};
