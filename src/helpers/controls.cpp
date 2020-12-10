#include "../../include/helpers/controls.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Controller::Controller(GLFWwindow *window, glm::vec3 initialPosition, float initialHorizontalAngle, float initialVerticalAngle, float fov, float speed, float sens, int width, int height)
{
    this->window = window;
    this->position = initialPosition;
    this->horizontalAngle = initialHorizontalAngle;
    this->verticalAngle = initialVerticalAngle;
    this->fov = fov;
    this->speed = speed;
    this->sens = sens;
    this->width = width;
    this->height = height;
}

glm::mat4 Controller::GetProjectionMatrix()
{
    return this->projection_matrix;
}

glm::mat4 Controller::GetViewMatrix()
{
    return this->view_matrix;
}

glm::mat4 Controller::GetMVP()
{
    return this->projection_matrix * this->view_matrix * glm::mat4(1.0);
}

void Controller::Update(float deltaTime)
{
    double xpos, ypos;
    glfwGetCursorPos(this->window, &xpos, &ypos);

    glfwSetCursorPos(this->window, this->width / 2, this->height / 2);

    this->horizontalAngle += this->sens * deltaTime * float(this->width / 2 - xpos);
    this->verticalAngle += this->sens * deltaTime * float(this->height / 2 - ypos);

    // Direction: Spherical cordinates to Cartesian coordinates conversion
    glm::vec3 direction(
        cos(this->verticalAngle) * sin(this->horizontalAngle),
        sin(this->verticalAngle),
        cos(this->verticalAngle) * cos(this->horizontalAngle));

    // Right vector
    glm::vec3 right = glm::vec3(sin(this->horizontalAngle - 3.14f / 2.0f), 0, cos(this->horizontalAngle - 3.14f / 2.0f));

    // Up vector: perpendicular to both direction and right
    glm::vec3 up = glm::cross(right, direction);
    // Move forward
    if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS)
    {
        position += direction * deltaTime * speed;
    }
    // Move backward
    if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS)
    {
        position -= direction * deltaTime * speed;
    }
    // Strafe right
    if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS)
    {
        position += right * deltaTime * speed;
    }
    // Strafe left
    if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS)
    {
        position -= right * deltaTime * speed;
    }
    // Strafe up
    if (glfwGetKey(this->window, GLFW_KEY_E) == GLFW_PRESS)
    {
        position += up * deltaTime * speed;
    }
    // Strafe down
    if (glfwGetKey(this->window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        position -= up * deltaTime * speed;
    }

    // Projection matrix
    this->projection_matrix = glm::perspective(this->fov, 4.0f / 3.0f, 0.1f, 100.0f);
    this->view_matrix = glm::lookAt(this->position, this->position + direction, up);
}