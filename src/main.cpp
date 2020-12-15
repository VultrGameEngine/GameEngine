#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <array>
#include "../include/core/core.h"
#include "../vendor/imgui/imgui.h"
#include "../vendor/imgui/imgui_impl_glfw.h"
#include "../vendor/imgui/imgui_impl_opengl3.h"

int main(void)
{
    World::Init();
    World::RegisterComponent<StaticMeshComponent>();
    World::RegisterComponent<TextureComponent>();
    World::RegisterComponent<TransformComponent>();
    World::RegisterComponent<LightComponent>();
    World::RegisterComponent<ShaderComponent>();
    World::RegisterComponent<CameraComponent>();
    World::RegisterComponent<ControllerComponent>();
    World::RegisterComponent<SkyBoxComponent>();
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
            Entity xwing = World::CreateEntity();

            World::AddComponent(
                xwing, StaticMeshComponent{
                           .path = "res/models/XWing.obj",
                       });
            World::AddComponent(
                xwing, TransformComponent{
                           .position = glm::vec3(i * 3, 0, j * 5)});
            World::AddComponent(
                xwing, TextureComponent{
                           .path = "res/textures/XWing.png",
                       });
            World::AddComponent(
                xwing, ShaderComponent{
                           .shader_path = "res/shaders/material.glsl",
                       });
        }
    }

    Entity camera = World::CreateEntity();

    World::AddComponent(
        camera, CameraComponent{
                    .enabled = true,
                });

    World::AddComponent(
        camera, TransformComponent{
                    .position = glm::vec3(4, 4, 4)});

    World::AddComponent(camera, ControllerComponent{});
    World::AddComponent(camera, SkyBoxComponent{
                                    .identifier = "default",
                                    .front = "res/textures/skybox/front.jpg",
                                    .back = "res/textures/skybox/back.jpg",
                                    .top = "res/textures/skybox/top.jpg",
                                    .bottom = "res/textures/skybox/bottom.jpg",
                                    .left = "res/textures/skybox/left.jpg",
                                    .right = "res/textures/skybox/right.jpg",
                                });
    World::AddComponent(camera, ShaderComponent{
                                    .shader_path = "res/shaders/skybox.glsl",
                                });
    Entity light = World::CreateEntity();

    World::AddComponent(
        light, LightComponent{});

    World::AddComponent(
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
    glfwSetWindowFocusCallback(window, ControllerSystem::WindowFocusCallback);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init((char *)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));

    ImGui::StyleColorsDark();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        float t = glfwGetTime();
        float deltaTime = t - lastTime;
        lastTime = t;
        double fps = 1.0f / deltaTime;
        std::cout << "FPS:" << fps << std::endl;

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ControllerSystem::Get()->Update(deltaTime);
        MeshLoaderSystem::Get()->Update();
        TextureLoaderSystem::Get()->Update();
        ShaderLoaderSystem::Get()->InitShaders();
        RenderSystem::Get()->Update(t);

        /* Poll for and process events */
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();

        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text."); // Display some text (you can use a format strings too)
        // ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our window open/close state
        // ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f); // Edit 1 float using a slider from 0.0f to 1.0f
        // ImGui::ColorEdit3("clear color", (float *)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
        ImGui::Begin("Another Window");
        ImGui::Text("Hello from another window!");
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        GLFWwindow *backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}
