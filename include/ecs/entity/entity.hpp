#pragma once
#include <stdint.h>

// An entity is just an ID
// It has a bunch of operator overloads so that it can be treated as such, so that we
// don't have to manually write out the id every single time we need to compare
// class Entity
// {
//   public:
//     Entity(uint32_t p_id) : id(p_id)
//     {
//     }

//     Entity(const Entity &entity)
//     {
//         id = entity.id;
//     }

//     bool operator==(const Entity &other)
//     {
//         return this->id == other.id;
//     }

//     bool operator!=(const Entity &other)
//     {
//         return this->id != other.id;
//     }

//     bool operator>(const Entity &other)
//     {
//         return this->id > other.id;
//     }

//     bool operator<(const Entity &other)
//     {
//         return this->id < other.id;
//     }

//     bool operator>=(const Entity &other)
//     {
//         return this->id >= other.id;
//     }

//     bool operator<=(const Entity &other)
//     {
//         return this->id <= other.id;
//     }

//     uint32_t id;
// };
using Entity = uint32_t;

// Used to define the size of the arrays to hold all of these entities
const Entity MAX_ENTITIES = 100000;
