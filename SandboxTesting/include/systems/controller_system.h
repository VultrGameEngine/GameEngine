#pragma once
#include <system_providers/controller_system_provider.h>

class ControllerSystem
{
  public:
    static void RegisterSystem();
    static void OnCreateEntity(Entity entity);
    static void OnDestroyEntity(Entity entity);
    static void Init(const glm::vec3 &center, GLFWwindow *window);
    static void OnScroll(GLFWwindow *window, double xoffset, double yoffset);
    static void Update(float delta_time);

  protected:
    static ControllerSystemProvider &GetProvider()
    {
        return *ControllerSystemProvider::Get();
    }
};
