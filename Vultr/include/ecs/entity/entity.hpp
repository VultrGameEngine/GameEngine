#pragma once
#include <cstddef>
#include <stdint.h>
#include <string>
#include <memory>
#include <ecs/component/component.hpp>
#include <yaml-cpp/yaml.h>

// An entity is just an ID
// It has a bunch of operator overloads so that it can be treated as such, so
// that we don't have to manually write out the id every single time we need to
// compare
class Entity
{
  public:
    Entity() = default;
    ~Entity() = default;

    Entity(int p_id) : id(p_id)
    {
    }

    bool operator==(const Entity &other) const
    {
        return this->id == other.id;
    }

    bool operator!=(const Entity &other) const
    {
        return this->id != other.id;
    }

    bool operator>(const Entity &other) const
    {
        return this->id > other.id;
    }

    bool operator<(const Entity &other) const
    {
        return this->id < other.id;
    }

    bool operator>=(const Entity &other) const
    {
        return this->id >= other.id;
    }

    bool operator<=(const Entity &other) const
    {
        return this->id <= other.id;
    }

    Signature GetSignature();

    template <class Archive> void serialize(Archive &archive)
    {
        archive(id); // serialize things by passing them to the archive
    }

    static Entity New();
    template <typename T> void AddComponent(T component);
    template <typename T> void RemoveComponent();
    template <typename T> T &GetComponent();
    template <typename T> bool HasComponent();

    // ONLY FOR THE EDITOR
    template <typename T> T *GetComponentUnsafe();

    int id;
};

// Used to define the size of the arrays to hold all of these entities
const size_t MAX_ENTITIES = 100000;

namespace std
{

template <> struct hash<Entity>
{
    std::size_t operator()(const Entity &e) const
    {
        using std::hash;
        using std::size_t;

        return hash<int>()(e.id);
    }
};

} // namespace std
