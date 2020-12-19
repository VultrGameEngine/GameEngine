#include "../../../include/core/systems/controller_system.h"
#include "../../../include/core/systems/render_system.h"
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>

static std::shared_ptr<ControllerSystem> instance;
void ControllerSystem::Init(GLFWwindow *window)
{
    this->window = window;
}

std::shared_ptr<ControllerSystem> ControllerSystem::Get()
{
    if (instance == 0)
    {
        instance = ControllerSystem::RegisterSystem();
    }
    return instance;
}

void ControllerSystem::Update(float delta_time)
{
    if (!this->focused)
        return;
    if (!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT))
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        return;
    }
    glm::vec2 dimensions = RenderSystem::GetDimensions(GAME);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    glfwSetCursorPos(window, dimensions.x / 2, dimensions.y / 2);

    Signature signature;
    signature.set(World::GetComponentType<ControllerComponent>());
    signature.set(World::GetComponentType<TransformComponent>());
    for (Entity entity : World::GetEntities(signature))
    {
        auto &transform_component = World::GetComponent<TransformComponent>(entity);
        auto &controller_component = World::GetComponent<ControllerComponent>(entity);
        glm::quat rotation_horiz = glm::angleAxis(controller_component.sens * delta_time * float(dimensions.x / 2 - xpos), glm::vec3(0, 1, 0));
        glm::quat rotation_vert = glm::angleAxis(controller_component.sens * delta_time * float(dimensions.y / 2 - ypos), transform_component.Right());
        transform_component.rotation = rotation_horiz * rotation_vert * transform_component.rotation;

        glm::vec3 speed = glm::vec3(3, 3, 3);

        // Move forward
        if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS)
        {
            transform_component.position += transform_component.Forward() * delta_time * speed;
        }
        // Move backward
        if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS)
        {
            transform_component.position -= transform_component.Forward() * delta_time * speed;
        }
        // Strafe right
        if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS)
        {
            transform_component.position += transform_component.Right() * delta_time * speed;
        }
        // Strafe left
        if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS)
        {
            transform_component.position -= transform_component.Right() * delta_time * speed;
        }
        // Strafe up
        if (glfwGetKey(this->window, GLFW_KEY_E) == GLFW_PRESS)
        {
            transform_component.position += transform_component.Up() * delta_time * speed;
        }
        // Strafe down
        if (glfwGetKey(this->window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            transform_component.position -= transform_component.Up() * delta_time * speed;
        }
    }
}
void ControllerSystem::WindowFocusCallback(GLFWwindow *window, int focused)
{
    instance->focused = focused;
    if (instance->focused)
    {
        glfwSetCursorPos(window, RenderSystem::GetDimensions(GAME).x / 2, RenderSystem::GetDimensions(GAME).y / 2);
    }
}

std::shared_ptr<ControllerSystem> ControllerSystem::RegisterSystem()
{
    std::shared_ptr<ControllerSystem> ptr = World::RegisterSystem<ControllerSystem>();
    return ptr;
}