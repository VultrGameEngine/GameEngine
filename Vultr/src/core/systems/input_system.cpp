#include <core/systems/input_system.h>
#include <core/systems/gui_system.h>
#include <core/system_providers/render_system_provider.h>
#include <engine.hpp>

namespace Vultr
{
void InputSystem::RegisterSystem()
{
    Signature signature;
    Engine::RegisterGlobalSystem<InputSystemProvider>(signature);
}
void InputSystem::OnMouseInput(GLFWwindow *window, int button, int action, int mods)
{
    InputSystemProvider &provider = GetProvider();
    provider.mouse_down = action == GLFW_PRESS;
    Input::MouseButtonInputEvent event =
        Input::MouseButtonInputEvent(provider.mouse_pos, provider.mouse_down);
    if (!GUISystem::ReceiveMouseButtonEvent(event))
    {
    }
}

void InputSystem::OnScroll(GLFWwindow *window, double xamount, double yamount)
{
    InputSystemProvider &provider = GetProvider();
    Input::ScrollInputEvent event =
        Input::ScrollInputEvent(provider.mouse_pos, glm::vec2(xamount, yamount));
    provider.AddScrollInput(glm::vec2(xamount, yamount));
    if (!GUISystem::ReceiveScrollEvent(event))
    {
        std::cout << "GUI did not receive scroll event" << std::endl;
    }
}

void InputSystem::Init(GLFWwindow *window)
{
    InputSystemProvider &provider = GetProvider();
    provider.window = window;
    glfwSetMouseButtonCallback(provider.window, OnMouseInput);
    glfwSetScrollCallback(provider.window, OnScroll);
}

void InputSystem::Update(const UpdateTick &tick)
{
    InputSystemProvider &provider = GetProvider();
    double posx, posy;
    if (!tick.debug)
    {
        glfwGetCursorPos(provider.window, &posx, &posy);
        const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        posx /= mode->width;
        posy /= mode->height;
        posy = 1 - posy;
    }
    else
    {
        ImVec2 mouse_pos = ImGui::GetMousePos();
        posx = mouse_pos.x - RenderSystemProvider::Get()->game.position.x;
        posy = mouse_pos.y - RenderSystemProvider::Get()->game.position.y;
        posx /= RenderSystemProvider::GetDimensions(GAME).x;
        posy /= RenderSystemProvider::GetDimensions(GAME).y;
        posy = 1 - posy;
        if (posx > 1 || posy > 1 || posx < 0 || posy < 0)
        {
            ImVec2 mouse_pos = ImGui::GetMousePos();
            posx = mouse_pos.x - RenderSystemProvider::Get()->scene.position.x;
            posy = mouse_pos.y - RenderSystemProvider::Get()->scene.position.y;
            posx /= RenderSystemProvider::GetDimensions(SCENE).x;
            posy /= RenderSystemProvider::GetDimensions(SCENE).y;
            posy = 1 - posy;
            glm::vec2 n_mouse_pos = glm::vec2(posx, posy);

            if (n_mouse_pos.x <= 1 && n_mouse_pos.y <= 1 && n_mouse_pos.x >= 0 &&
                n_mouse_pos.y >= 0)
            {
                provider.scene_mouse_pos = n_mouse_pos;
            }
            return;
        }
    }
    glm::vec2 n_mouse_pos = glm::vec2(posx, posy);

    if (provider.mouse_pos != n_mouse_pos)
    {
        std::cout << "Within bounds" << std::endl;
        // Send mouse event to the gui system and see if it does anything
        provider.mouse_pos = n_mouse_pos;
        provider.mouse_down =
            glfwGetMouseButton(provider.window, GLFW_MOUSE_BUTTON_LEFT) ==
            GLFW_PRESS;
        Input::MouseInputEvent event =
            Input::MouseInputEvent(provider.mouse_pos, provider.mouse_down);
        if (!GUISystem::ReceiveMouseEvent(event))
        {
        }
    }
}

} // namespace Vultr
