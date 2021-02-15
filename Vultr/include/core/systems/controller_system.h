// Controller system handles input from the user translated to transform changes

#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <core/components/controller_component.h>
#include <core/components/transform_component.h>
#include <core/system_providers/controller_system_provider.h>
#include <ecs/system/system.hpp>
#include <ecs/world/world.hpp>
#include <glm/glm.hpp>
#include <memory>

namespace Brick3D
{

class ControllerSystem : public System
{
  public:
    static void Init(GLFWwindow *window);
    static void Update(float delta_time);
    static void RegisterSystem();
    static void WindowFocusCallback(GLFWwindow *window, int focused);

  protected:
    SystemProvider &GetProvider() override
    {
        return ControllerSystemProvider::Get();
    }
};
} // namespace Brick3D
