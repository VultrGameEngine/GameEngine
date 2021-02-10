#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <array>
#include <core/core.h>
#include <editor/editor.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <stdio.h>

using namespace Brick3D;
int main(void)
{
    World::Init();
    World::RegisterComponent<StaticMeshComponent>();
    World::RegisterComponent<MaterialComponent>();
    World::RegisterComponent<PBRMaterial>();
    World::RegisterComponent<TransformComponent>();
    World::RegisterComponent<LightComponent>();
    World::RegisterComponent<CameraComponent>();
    World::RegisterComponent<ControllerComponent>();
    World::RegisterComponent<SkyBoxComponent>();
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1920, 1080, "GameEngine", nullptr, nullptr);
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
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(ErrorHandler::ErrorCallback, 0);

    ControllerSystem::Init(window);
    glfwSetWindowFocusCallback(window, ControllerSystem::WindowFocusCallback);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport /
                                                        // Platform Windows
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init((char *)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));

    ImGui::StyleColorsDark();

    MeshLoaderSystem::RegisterSystem();
    ShaderLoaderSystem::RegisterSystem();
    TextureLoaderSystem::RegisterSystem();
    ControllerSystem::RegisterSystem();
    CameraSystem::RegisterSystem();
    LightSystem::RegisterSystem();
    RenderSystem::RegisterSystem();

    // for (int i = 0; i < 10; i++) {
    //   for (int j = 0; j < 10; j++) {
    // for (int k = 0; k < 10; k++) {
    Entity xwing = World::CreateEntity();

    World::AddComponent(xwing, StaticMeshComponent("res/models/Clone.obj"));
    World::AddComponent(xwing, TransformComponent{
                                   .position = glm::vec3(0, 0, 0),
                                   .scale = glm::vec3(5, 5, 5),
                               });
    World::AddComponent(xwing, PBRMaterial("res/shaders/material.glsl",
                                           "res/textures/clone/albedo.jpeg",
                                           "res/textures/clone/specular.jpeg"));
    // }
    // }
    // }

    Entity camera = World::CreateEntity();

    World::AddComponent(camera, CameraComponent{
                                    .enabled = true,
                                });

    World::AddComponent(camera, TransformComponent{.position = glm::vec3(4, 4, 4)});

    World::AddComponent(camera, ControllerComponent{
                                    .sens = 0.05,
                                });
    World::AddComponent(camera, SkyBoxComponent{
                                    .identifier = "default",
                                    .front = "res/textures/skybox/front.jpg",
                                    .back = "res/textures/skybox/back.jpg",
                                    .top = "res/textures/skybox/top.jpg",
                                    .bottom = "res/textures/skybox/bottom.jpg",
                                    .left = "res/textures/skybox/left.jpg",
                                    .right = "res/textures/skybox/right.jpg",
                                });
    // World::AddComponent(camera, ShaderComponent{
    //                                 .path = "res/shaders/skybox.glsl",
    //                             });
    Entity light = World::CreateEntity();

    World::AddComponent(light, LightComponent{});

    World::AddComponent(light, TransformComponent{
                                   .position = glm::vec3(0, 0, 2),
                                   .scale = glm::vec3(0.2, 0.2, 0.2),
                               });
    // World::AddComponent(light, ShaderComponent{
    //                                .path = "res/shaders/unlit.glsl",
    //                                .type = Renderer::Forward,
    //                            });
    World::AddComponent(light, StaticMeshComponent("res/models/cube.obj"));

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        float t = glfwGetTime();
        float deltaTime = t - lastTime;
        lastTime = t;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ControllerSystem::Update(deltaTime);
        TextureLoaderSystem::Update();
        LightSystem::Update();
        RenderSystem::Update(UpdateTick(t));

        /* Poll for and process events */
        glfwPollEvents();
        Brick3D::Editor::Editor::Get()->Render();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}
