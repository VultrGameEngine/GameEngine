// Controller system handles input from the user translated to transform changes

#pragma once
#include <GLFW/glfw3.h>
#include <core/components/controller_component.h>
#include <core/components/transform_component.h>
#include <core/system_providers/controller_system_provider.h>
#include <glm/glm.hpp>
#include <memory>

namespace Vultr
{

class ControllerSystem
{
  public:
    static void Init(GLFWwindow *window);
    static void Update(float delta_time);
    static void RegisterSystem();
    static void WindowFocusCallback(GLFWwindow *window, int focused);

  protected:
    SystemProvider &GetProvider()
    {
        std::shared_ptr<ControllerSystemProvider> provider = ControllerSystemProvider::Get();
        return *provider;
    }
};
} // namespace Vultr
