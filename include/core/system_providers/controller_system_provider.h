#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <ecs/system/system_provider.hpp>

namespace Brick3D
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
    Signature signature;
};
} // namespace Brick3D
