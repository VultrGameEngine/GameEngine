#include <Vultr.hpp>
#include <stdio.h>

using namespace Brick3D;
class TestGame : public Game
{
  public:
    TestGame(Vultr *engine)
    {
        std::cout << "Test Game created... " << std::endl;
    }

    void Init() override
    {
        std::cout << "Test Game initialized... " << std::endl;
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
    }

    void Update(Brick3D::UpdateTick tick) override
    {
        // std::cout << "Update!" << std::endl;
    }

    void Flush() override
    {
        std::cout << "Shutting down Test Game" << std::endl;
    }

  private:
    Vultr *engine;
};

extern "C"
{
    Game *init(Vultr *engine)
    {
        return new TestGame(engine);
    }
    void flush(Game *game)
    {
        delete game;
    }
}
