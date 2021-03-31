#pragma once
#include <core/system_providers/input_system_provider.h>

namespace Vultr
{
class InputSystem
{
  public:
    static void RegisterSystem();
    static void Init(GLFWwindow *window);
    static void OnMouseInput(GLFWwindow *window, int button, int action, int mods);
    static void Update();

  protected:
    static InputSystemProvider &GetProvider()
    {
        return *InputSystemProvider::Get();
    }
};

} // namespace Vultr
