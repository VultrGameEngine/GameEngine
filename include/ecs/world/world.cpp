#include "world.hpp"

World *World::instance = 0;

World *World::Get()
{

    if (instance == 0)
    {
        instance = new World();
        instance->Init();
    }
    return instance;
}