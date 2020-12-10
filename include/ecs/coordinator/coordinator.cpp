#include "coordinator.hpp"

Coordinator *Coordinator::instance = 0;

Coordinator *Coordinator::Get()
{

    if (instance == 0)
    {
        instance = new Coordinator();
        instance->Init();
    }
    return instance;
}