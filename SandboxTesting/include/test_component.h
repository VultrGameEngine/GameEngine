#pragma once
#include <memory>
#include <ecs/entity/entity.hpp>
#include <core/component_renderer.h>

struct TestComponent
{
    static std::shared_ptr<TestComponent> Create()
    {
        return std::make_shared<TestComponent>();
    }

    int x = 0;

    template <class Archive> void serialize(Archive &ar)
    {
        ar(x);
    }
};

VULTR_REGISTER_COMPONENT(TestComponent, x);

void RenderTestComponent(Entity entity)
{
}
