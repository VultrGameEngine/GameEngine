#include <core/systems/input_system.h>
#include <core/systems/gui_system.h>

namespace Vultr
{
void InputSystem::RegisterSystem()
{
    Signature signature;
    World::RegisterSystem<InputSystemProvider>(signature);
}
void InputSystem::OnMouseInput(GLFWwindow *window, int button, int action, int mods)
{
    InputSystemProvider &provider = GetProvider();
    provider.mouse_down = action == GLFW_PRESS;
    Input::MouseButtonInputEvent event =
        Input::MouseButtonInputEvent(provider.mouse_pos, provider.mouse_down);
    if (!GUISystem::ReceiveMouseButtonEvent(event))
    {
        std::cout << "GUI did not receive mouse button event " << provider.mouse_down
                  << std::endl;
    }
}

void InputSystem::Init(GLFWwindow *window)
{
    InputSystemProvider &provider = GetProvider();
    provider.window = window;
    glfwSetMouseButtonCallback(provider.window, OnMouseInput);
}

void InputSystem::Update()
{
    InputSystemProvider &provider = GetProvider();
    double posx, posy;
    glfwGetCursorPos(provider.window, &posx, &posy);
    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    posx /= mode->width;
    posy /= mode->height;
    posy = 1 - posy;
    glm::vec2 n_mouse_pos = glm::vec2(posx, posy);

    if (provider.mouse_pos != n_mouse_pos)
    {
        // Send mouse event to the gui system and see if it does anything
        provider.mouse_pos = n_mouse_pos;
        provider.mouse_down =
            glfwGetMouseButton(provider.window, GLFW_MOUSE_BUTTON_LEFT) ==
            GLFW_PRESS;
        Input::MouseInputEvent event =
            Input::MouseInputEvent(provider.mouse_pos, provider.mouse_down);
        if (!GUISystem::ReceiveMouseEvent(event))
        {
            // std::cout << "GUI did not receive mouse event " << provider.mouse_down
            //           << std::endl;
        }
    }
}

} // namespace Vultr
