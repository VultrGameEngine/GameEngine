#pragma once
#include <Vultr.hpp>

VCOMPONENT()
struct TestComponent
{
    TestComponent() = default;

    static TestComponent Create()
    {
        return TestComponent();
    }

    VPROPERTY()
    bool some_bool = false;

    VPROPERTY()
    double some_double = 10.5;

    VPROPERTY()
    int some_int = 1;
};
