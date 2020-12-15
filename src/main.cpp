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
    World::Get()->Init();
    World::Get()->RegisterComponent<StaticMeshComponent>();
    World::Get()->RegisterComponent<TextureComponent>();
    World::Get()->RegisterComponent<TransformComponent>();
    World::Get()->RegisterComponent<LightComponent>();
    World::Get()->RegisterComponent<ShaderComponent>();
    World::Get()->RegisterComponent<CameraComponent>();
    World::Get()->RegisterComponent<ControllerComponent>();
    World::Get()->RegisterComponent<SkyBoxComponent>();
    RenderSystem::Get();
    MeshLoaderSystem::Get();
    ShaderLoaderSystem::Get();
    TextureLoaderSystem::Get();
    ControllerSystem::Get();
    CameraSystem::Get();

    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            Entity xwing = World::Get()->CreateEntity();

            World::Get()->AddComponent(
                xwing, StaticMeshComponent{
                           .path = "res/models/XWing.obj",
                       });
            World::Get()->AddComponent(
                xwing, TransformComponent{
                           .position = glm::vec3(i * 3, 0, j * 5)});
            World::Get()->AddComponent(
                xwing, TextureComponent{
                           .path = "res/textures/XWing.png",
                       });
            World::Get()->AddComponent(
                xwing, ShaderComponent{
                           .shader_path = "res/shaders/material.glsl",
                       });
        }
    }

    Entity camera = World::Get()->CreateEntity();

    World::Get()->AddComponent(
        camera, CameraComponent{
                    .enabled = true,
                });

    World::Get()->AddComponent(
        camera, TransformComponent{
                    .position = glm::vec3(4, 4, 4)});

    World::Get()->AddComponent(camera, ControllerComponent{});
    World::Get()->AddComponent(camera, SkyBoxComponent{
                                           .identifier = "default",
                                           .front = "res/textures/skybox/front.jpg",
                                           .back = "res/textures/skybox/back.jpg",
                                           .top = "res/textures/skybox/top.jpg",
                                           .bottom = "res/textures/skybox/bottom.jpg",
                                           .left = "res/textures/skybox/left.jpg",
                                           .right = "res/textures/skybox/right.jpg",
                                       });
    World::Get()->AddComponent(camera, ShaderComponent{
                                           .shader_path = "res/shaders/skybox.glsl",
                                       });
    Entity light = World::Get()->CreateEntity();

    World::Get()->AddComponent(
        light, LightComponent{});

    World::Get()->AddComponent(
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
    // glEnable(GL_DEPTH_BUFFER);

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
