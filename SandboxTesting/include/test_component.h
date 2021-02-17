#pragma once
#include <memory>
#include <ecs/entity/entity.hpp>

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

void RenderTestComponent(Entity entity)
{
}
