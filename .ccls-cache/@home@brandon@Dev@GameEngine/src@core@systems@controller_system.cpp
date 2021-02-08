#include <core/systems/controller_system.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Brick3D
{

void ControllerSystem::Init(GLFWwindow *window)
{
    ControllerSystemProvider::Get().m_window = window;
}

void ControllerSystem::Update(float delta_time)
{
    ControllerSystemProvider &provider = ControllerSystemProvider::Get();
    if (!provider.m_focused)
        return;
    if (!glfwGetMouseButton(provider.m_window, GLFW_MOUSE_BUTTON_RIGHT))
    {
        glfwSetInputMode(provider.m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        return;
    }
    glm::vec2 dimensions = RenderSystem::GetDimensions(GAME);

    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    double xpos, ypos;
    glfwGetCursorPos(provider.m_window, &xpos, &ypos);

    glfwSetCursorPos(provider.m_window, dimensions.x / 2, dimensions.y / 2);

    for (Entity entity : provider.entities)
    {
        auto &transform_component = World::GetComponent<TransformComponent>(entity);
        auto &controller_component = World::GetComponent<ControllerComponent>(entity);
        glm::quat rotation_horiz =
            glm::angleAxis(controller_component.sens * delta_time * float(dimensions.x / 2 - xpos), glm::vec3(0, 1, 0));
        glm::quat rotation_vert = glm::angleAxis(
            controller_component.sens * delta_time * float(dimensions.y / 2 - ypos), transform_component.Right());
        transform_component.rotation = rotation_horiz * rotation_vert * transform_component.rotation;

        glm::vec3 speed = glm::vec3(3, 3, 3);

        // Move forward
        if (glfwGetKey(provider.m_window, GLFW_KEY_W) == GLFW_PRESS)
        {
            transform_component.position += transform_component.Forward() * delta_time * speed;
        }
        // Move backward
        if (glfwGetKey(provider.m_window, GLFW_KEY_S) == GLFW_PRESS)
        {
            transform_component.position -= transform_component.Forward() * delta_time * speed;
        }
        // Strafe right
        if (glfwGetKey(provider.m_window, GLFW_KEY_D) == GLFW_PRESS)
        {
            transform_component.position += transform_component.Right() * delta_time * speed;
        }
        // Strafe left
        if (glfwGetKey(provider.m_window, GLFW_KEY_A) == GLFW_PRESS)
        {
            transform_component.position -= transform_component.Right() * delta_time * speed;
        }
        // Strafe up
        if (glfwGetKey(provider.m_window, GLFW_KEY_E) == GLFW_PRESS)
        {
            transform_component.position += transform_component.Up() * delta_time * speed;
        }
        // Strafe down
        if (glfwGetKey(provider.m_window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            transform_component.position -= transform_component.Up() * delta_time * speed;
        }
    }
}
void ControllerSystem::WindowFocusCallback(GLFWwindow *window, int focused)
{
    ControllerSystemProvider &provider = ControllerSystemProvider::Get();
    provider.m_focused = focused;
    if (provider.m_focused)
    {
        glfwSetCursorPos(provider.m_window, RenderSystem::GetDimensions(GAME).x / 2,
                         RenderSystem::GetDimensions(GAME).y / 2);
    }
}

void ControllerSystem::RegisterSystem()
{
    ControllerSystemProvider &provider = ControllerSystemProvider::Get();
    provider.signature.set(World::GetComponentType<CameraComponent>(), true);
    provider.signature.set(World::GetComponentType<TransformComponent>(), true);
    std::shared_ptr<ControllerSystem> ptr = World::RegisterSystem<ControllerSystem>(provider.signature);
}
} // namespace Brick3D
