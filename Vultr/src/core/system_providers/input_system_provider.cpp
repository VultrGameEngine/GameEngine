#include <core/system_providers/input_system_provider.h>
#include <core/systems/input_system.h>

namespace Vultr
{
void InputSystemProvider::OnCreateEntity(Entity entity)
{
}

void InputSystemProvider::OnDestroyEntity(Entity entity)
{
}

bool InputSystemProvider::KeyDown(const char key) const
{
    int state = glfwGetKey(window, std::toupper(key));
    return state == GLFW_PRESS;
}

} // namespace Vultr
