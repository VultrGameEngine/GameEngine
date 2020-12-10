#include "../../../include/core/systems/controller_system.h"
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>

std::shared_ptr<ControllerSystem> ControllerSystem::instance = 0;
Signature ControllerSystem::signature = 0;

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
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    glfwSetCursorPos(window, width / 2, height / 2);

    // this->horizontalAngle += ;
    // this->verticalAngle += this->sens * deltaTime * float(this->height / 2 - ypos);

    // Direction: Spherical cordinates to Cartesian coordinates conversion
    // glm::vec3 direction(
    //     cos(this->verticalAngle) * sin(this->horizontalAngle),
    //     sin(this->verticalAngle),
    //     cos(this->verticalAngle) * cos(this->horizontalAngle));

    // // Right vector
    // glm::vec3 right = glm::vec3(sin(this->horizontalAngle - 3.14f / 2.0f), 0, cos(this->horizontalAngle - 3.14f / 2.0f));

    // // Up vector: perpendicular to both direction and right
    // glm::vec3 up = glm::cross(right, direction);

    // // Move forward
    // if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS)
    // {
    //     position += direction * deltaTime * speed;
    // }
    // // Move backward
    // if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS)
    // {
    //     position -= direction * deltaTime * speed;
    // }
    // // Strafe right
    // if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS)
    // {
    //     position += right * deltaTime * speed;
    // }
    // // Strafe left
    // if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS)
    // {
    //     position -= right * deltaTime * speed;
    // }
    // // Strafe up
    // if (glfwGetKey(this->window, GLFW_KEY_E) == GLFW_PRESS)
    // {
    //     position += up * deltaTime * speed;
    // }
    // // Strafe down
    // if (glfwGetKey(this->window, GLFW_KEY_Q) == GLFW_PRESS)
    // {
    //     position -= up * deltaTime * speed;
    // }

    for (Entity entity : Coordinator::Get()->GetEntities(signature))
    {
        auto &transform_component = Coordinator::Get()->GetComponent<TransformComponent>(entity);
        auto &controller_component = Coordinator::Get()->GetComponent<ControllerComponent>(entity);
        glm::quat rotation_horiz = glm::angleAxis(controller_component.sens * delta_time * float(width / 2 - xpos), glm::vec3(0, 1, 0));
        transform_component.rotation *= rotation_horiz;
    }
}

std::shared_ptr<ControllerSystem> ControllerSystem::RegisterSystem()
{
    std::shared_ptr<ControllerSystem> ptr = Coordinator::Get()->RegisterSystem<ControllerSystem>();
    signature.set(Coordinator::Get()->GetComponentType<ControllerComponent>());
    signature.set(Coordinator::Get()->GetComponentType<TransformComponent>());
    Coordinator::Get()->SetSystemSignature<ControllerSystem>(signature);
    return ptr;
}