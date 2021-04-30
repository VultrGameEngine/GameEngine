#pragma once
#include <assert.h>
#include <ecs/entity/entity.hpp>
#include <ecs/component/component.hpp>
#include <set>

template <typename T> std::string GetName();

#define VultrRegisterSystemProvider(T)                                              \
    template <> inline std::string GetName<T>()                                     \
    {                                                                               \
        return #T;                                                                  \
    }
namespace Vultr
{
struct SystemProvider
{
    // A list of entities related to a specific System
    // automatically managed by this class and the world
    std::set<Entity> entities;
    Signature signature;
};
} // namespace Vultr
