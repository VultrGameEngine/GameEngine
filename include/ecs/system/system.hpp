// A System is functionality that iterates upon a list of entities with a certain signature of components
// This System class is to be inherited by real system classes

#pragma once
#include <set>
#include "../entity/entity.hpp"
#include "../component/component_array.hpp"
#include "../component/component.hpp"
#include <cassert>
#include <memory>

class System
{
public:
    // TODO figure out a way to not make this virtual
    // This creates unnecessary overhead for a function that shouldn't need it and that is called often enough that it could cause an issue
    virtual void DestroyEntity(Entity entity)
    {
        assert("Destroy ENTITY WAS NOT IMPLEMENTED!!");
    }

    static Signature signature;
};
