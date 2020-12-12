#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <array>
#include "../include/models/Vertex.h"
#include "../include/helpers/controls.h"
#include "../include/models/Mesh.h"
#include "../include/core/core.h"

int main(void)
{
    Coordinator::Get()->Init();
    Coordinator::Get()->RegisterComponent<StaticMeshComponent>();
    Coordinator::Get()->RegisterComponent<TextureComponent>();
    Coordinator::Get()->RegisterComponent<TransformComponent>();
    Coordinator::Get()->RegisterComponent<LightComponent>();
    Coordinator::Get()->RegisterComponent<ShaderComponent>();
    Coordinator::Get()->RegisterComponent<CameraComponent>();
    Coordinator::Get()->RegisterComponent<ControllerComponent>();
    RenderSystem::Get();
    MeshLoaderSystem::Get();
    ShaderLoaderSystem::Get();
    TextureLoaderSystem::Get();
    ControllerSystem::Get();
    CameraSystem::Get();

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            Entity xwing = Coordinator::Get()->CreateEntity();

            Coordinator::Get()->AddComponent(
                xwing, StaticMeshComponent{
                           .path = "res/models/XWing.obj",
                       });
            Coordinator::Get()->AddComponent(
                xwing, TransformComponent{
                           .position = glm::vec3(i * 3, 0, j * 5)});
            Coordinator::Get()->AddComponent(
                xwing, TextureComponent{
                           .path = "res/textures/XWing.png",
                       });
            Coordinator::Get()->AddComponent(
                xwing, ShaderComponent{
                           .shader_path = "res/shaders/material.glsl",
                       });
        }
    }

    Entity camera = Coordinator::Get()->CreateEntity();

    Coordinator::Get()->AddComponent(
        camera, CameraComponent{
                    .enabled = true,
                });

    Coordinator::Get()->AddComponent(
        camera, TransformComponent{
                    .position = glm::vec3(4, 4, 4)});

    Coordinator::Get()->AddComponent(camera, ControllerComponent{});
    Entity light = Coordinator::Get()->CreateEntity();

    Coordinator::Get()->AddComponent(
        light, LightComponent{});

    Coordinator::Get()->AddComponent(
        light, TransformComponent{
                   .position = glm::vec3(4, 4, 4)});

    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1920, 1080, "GameEngine", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        printf("Failed to initialze glue\n");
        return -1;
    }

    float lastTime = 0;
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_BUFFER);

    ControllerSystem::Get()->Init(window, 1920, 1080);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glfwSetWindowFocusCallback(window, ControllerSystem::WindowFocusCallback);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        float t = glfwGetTime();
        float deltaTime = t - lastTime;
        lastTime = t;
        double fps = 1.0f / deltaTime;
        // std::cout << "FPS:" << fps << std::endl;

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ControllerSystem::Get()->Update(deltaTime);
        MeshLoaderSystem::Get()->Update();
        TextureLoaderSystem::Get()->Update();
        ShaderLoaderSystem::Get()->InitShaders();
        RenderSystem::Get()->Update(t);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}