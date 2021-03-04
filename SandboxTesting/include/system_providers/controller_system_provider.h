#pragma once
#include <Vultr.hpp>
#include <GLFW/glfw3.h>

class ControllerSystemProvider : public SystemProvider
{
  public:
    static std::shared_ptr<ControllerSystemProvider> Get()
    {
        return World::GetSystemProvider<ControllerSystemProvider>();
    }

    Entity controller = Entity(-1);
    TransformComponent center;
    GLFWwindow *window;
    float radius = 100;

  protected:
    void OnCreateEntity(Entity entity) override;
    void OnDestroyEntity(Entity entity) override;
};
