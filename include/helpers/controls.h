#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Controller
{
public:
    Controller(GLFWwindow *window, glm::vec3 initialPostion, float initialHorizontalAngle, float initialVerticalAngle, float fov, float speed, float sens, int width, int height);
    void Update(float);
    glm::mat4 GetProjectionMatrix();
    glm::mat4 GetViewMatrix();
    glm::mat4 GetMVP();

private:
    GLFWwindow *window;
    glm::mat4 projection_matrix;
    glm::mat4 view_matrix;

    // Settings for controller
    glm::vec3 position;
    float horizontalAngle;
    float verticalAngle;
    float fov;

    float speed;
    float sens;

    int width;
    int height;
};