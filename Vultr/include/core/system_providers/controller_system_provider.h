#pragma once
#include <GLFW/glfw3.h>
#include <ecs/system/system_provider.hpp>
#include <glad/glad.h>

namespace Vultr
{
class ControllerSystemProvider : public SystemProvider
{
  public:
    // Singleton pattern for all providers
    static ControllerSystemProvider &Get()
    {
        static ControllerSystemProvider instance;
        return instance;
    }
    GLFWwindow *m_window;
    bool m_focused = false;
};
} // namespace Vultr
