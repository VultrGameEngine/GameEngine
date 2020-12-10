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
    Coordinator::Get()->RegisterComponent<TransformComponent>();
    Coordinator::Get()->RegisterComponent<ShaderComponent>();
    Coordinator::Get()->RegisterComponent<CameraComponent>();
    Coordinator::Get()->RegisterComponent<ControllerComponent>();
    RenderSystem::Get();
    MeshLoaderSystem::Get();
    ShaderLoaderSystem::Get();
    ControllerSystem::Get();
    CameraSystem::Get();
    Entity xwing = Coordinator::Get()->CreateEntity();

    Coordinator::Get()->AddComponent(
        xwing, StaticMeshComponent{
                   .path = "res/models/XWing.obj",
               });
    Coordinator::Get()->AddComponent(
        xwing, TransformComponent{});
    Coordinator::Get()->AddComponent(
        xwing, ShaderComponent{
                   .shader_path = "res/shaders/Basic.shader",
               });

    Entity camera = Coordinator::Get()->CreateEntity();

    Coordinator::Get()->AddComponent(
        camera, CameraComponent{
                    .enabled = true,
                });

    Coordinator::Get()->AddComponent(
        camera, TransformComponent{
                    .position = glm::vec3(0, 0, 5)});

    Coordinator::Get()->AddComponent(camera, ControllerComponent{});

    Entity cube = Coordinator::Get()->CreateEntity();

    Coordinator::Get()->AddComponent(
        cube, StaticMeshComponent{
                  .path = "res/models/cube.obj"});
    Coordinator::Get()->AddComponent(
        cube, TransformComponent{});
    Coordinator::Get()->AddComponent(
        cube, ShaderComponent{
                  .shader_path = "res/shaders/Basic.shader",
              });

    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1920, 1080, "Joe", NULL, NULL);
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
    // Controller *controller = new Controller(window, glm::vec3(0, 0, 0), 3.14f, 0.0f, 45.0f, 3.0f, 1.0f, 1920, 1080);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_BUFFER);
    ControllerSystem::Get()->Init(window, 1920, 1080);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        float t = glfwGetTime();
        float deltaTime = t - lastTime;
        lastTime = t;

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // controller->Update(deltaTime);

        ControllerSystem::Get()->Update(deltaTime);
        MeshLoaderSystem::Get()->Update();
        ShaderLoaderSystem::Get()->InitShaders();
        CameraSystem::Get()->Update();
        RenderSystem::Get()->Update(t);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}