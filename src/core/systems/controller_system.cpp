#include "../../../include/core/systems/controller_system.h"
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>

std::shared_ptr<ControllerSystem> ControllerSystem::instance = 0;

void ControllerSystem::Init(GLFWwindow *window, int width, int height)
{
    this->window = window;
    this->width = width;
    this->height = height;
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
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    glfwSetCursorPos(window, width / 2, height / 2);

    Signature signature;
    signature.set(Coordinator::Get()->GetComponentType<ControllerComponent>());
    signature.set(Coordinator::Get()->GetComponentType<TransformComponent>());
    for (Entity entity : Coordinator::Get()->GetEntities(signature))
    {
        auto &transform_component = Coordinator::Get()->GetComponent<TransformComponent>(entity);
        auto &controller_component = Coordinator::Get()->GetComponent<ControllerComponent>(entity);
        glm::quat rotation_horiz = glm::angleAxis(controller_component.sens * delta_time * float(width / 2 - xpos), glm::vec3(0, 1, 0));
        glm::quat rotation_vert = glm::angleAxis(controller_component.sens * delta_time * float(height / 2 - ypos), transform_component.Right());
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
}

std::shared_ptr<ControllerSystem> ControllerSystem::RegisterSystem()
{
    std::shared_ptr<ControllerSystem> ptr = Coordinator::Get()->RegisterSystem<ControllerSystem>();
    return ptr;
}