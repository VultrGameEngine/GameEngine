#include <core/systems/input_system.h>

namespace Vultr
{
void InputSystem::RegisterSystem()
{
    Signature signature;
    World::RegisterSystem<InputSystemProvider>(signature);
}

void InputSystem::Update()
{
    InputSystemProvider &provider = GetProvider();
    double posx, posy;
    glfwGetCursorPos(provider.window, &posx, &posy);
    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    posx /= mode->width;
    posy /= mode->height;
    glm::vec2 n_mouse_pos = glm::vec2(posx, posy);

    if (provider.mouse_pos != n_mouse_pos)
    {
        // Send mouse event to the gui system and see if it does anything
        provider.mouse_pos = n_mouse_pos;
    }
}

} // namespace Vultr
