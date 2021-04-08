#pragma once
#include <core/system_providers/input_system_provider.h>
#include <core/models/update_tick.h>

namespace Vultr
{
class InputSystem
{
  public:
    static void RegisterSystem();
    static void Init(GLFWwindow *window);
    static void OnMouseInput(GLFWwindow *window, int button, int action, int mods);
    static void OnScroll(GLFWwindow *window, double xoffset, double yoffset);
    static void Update(const UpdateTick &tick);

  protected:
    static InputSystemProvider &GetProvider()
    {
        std::shared_ptr<InputSystemProvider> provider = InputSystemProvider::Get();
        return *provider;
    }
};

} // namespace Vultr
