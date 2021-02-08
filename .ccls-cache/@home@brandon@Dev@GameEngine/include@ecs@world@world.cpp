#include "world.hpp"

World *World::Get()
{
    static World *instance;

    if (instance == 0)
    {
        instance = new World();
    }
    return instance;
}