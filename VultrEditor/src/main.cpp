#include <Vultr.hpp>
#include <editor/editor.hpp>
#include <iostream>

using namespace Brick3D;
int main(void)
{
    Vultr *vultr = new Vultr();

    float lastTime = 0;
    vultr->Init(true);
    vultr->LoadGame("/home/brandon/Dev/GameEngine/SandboxTesting/build/libGame.so");
    Entity xwing = Entity::New();

    xwing.AddComponent(TransformComponent::Create(
        glm::vec3(0, 0, 0), glm::quat(1, 0, 0, 0), glm::vec3(1, 1, 1)));
    xwing.AddComponent(
        ForwardMaterial::Create("/home/brandon/Dev/GameEngine/SandboxTesting/"
                                "res/textures/clone/albedo.jpeg"));
    xwing.AddComponent(StaticMeshComponent::Create(
        "/home/brandon/Dev/GameEngine/SandboxTesting/res/models/Clone.obj"));

    Entity camera = Entity::New();
    camera.AddComponent(CameraComponent::Create());

    camera.AddComponent(TransformComponent::Create(
        glm::vec3(4, 4, 4), glm::quat(1, 0, 0, 0), glm::vec3(5, 5, 5)));

    camera.AddComponent(ControllerComponent::Create());
    camera.AddComponent(
        SkyBoxComponent::Create("default",
                                "/home/brandon/Dev/GameEngine/SandboxTesting/"
                                "res/textures/skybox/front.jpg",
                                "/home/brandon/Dev/GameEngine/SandboxTesting/"
                                "res/textures/skybox/back.jpg",
                                "/home/brandon/Dev/GameEngine/SandboxTesting/"
                                "res/textures/skybox/top.jpg",
                                "/home/brandon/Dev/GameEngine/SandboxTesting/"
                                "res/textures/skybox/bottom.jpg",
                                "/home/brandon/Dev/GameEngine/SandboxTesting/"
                                "res/textures/skybox/left.jpg",
                                "/home/brandon/Dev/GameEngine/SandboxTesting/"
                                "res/textures/skybox/right.jpg"));
    camera.AddComponent(SkyboxMaterial::Create("default"));
    Entity light = Entity::New();

    light.AddComponent(LightComponent::Create());

    light.AddComponent(TransformComponent::Create(
        glm::vec3(0, 0, 10), glm::quat(1, 0, 0, 0), glm::vec3(0.2, 0.2, 0.2)));
    light.AddComponent(UnlitMaterial::Create());
    light.AddComponent(StaticMeshComponent::Create(
        "/home/brandon/Dev/GameEngine/SandboxTesting/res/models/cube.obj"));

    while (true)
    {
        float t = glfwGetTime();
        float deltaTime = t - lastTime;
        lastTime = t;
        UpdateTick tick = UpdateTick(t);
        vultr->UpdateGame(tick);
        Brick3D::Editor::Editor::Get()->Render();

        // glfwSwapBuffers(window);
    }

    // ImGui_ImplOpenGL3_Shutdown();
    // ImGui_ImplGlfw_Shutdown();

    // ImGui::DestroyContext();
}
