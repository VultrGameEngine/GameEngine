#include <Vultr.hpp>
// #include <cereal/archives/binary.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/polymorphic.hpp>

int main(void)
{
    Vultr::Engine *vultr = new Vultr::Engine();

    float lastTime = 0;
    vultr->Init(false);
    Entity testEntityWithTransform = Entity::New();

    {
        testEntityWithTransform.AddComponent(TransformComponent::Create(
            glm::vec3(192, 150, 182), glm::quat(1, 0, 0, 0), glm::vec3(1, 1, 1)));
        testEntityWithTransform.AddComponent(ForwardMaterial::Create(
            "/home/brandon/Dev/GameEngine/SandboxTesting/res/"
            "textures/clone/albedo.jpeg"));
        std::cout << testEntityWithTransform.GetComponent<MaterialComponent>()
                         .GetTextures()
                         .at(0)
                  << std::endl;

        std::ofstream os("test_world.world");
        cereal::BinaryOutputArchive oarchive(os);

        oarchive(World::Get()->component_manager);
    }
    World::Init();
    {
        std::ifstream is("test_world.world");
        cereal::BinaryInputArchive iarchive(is);

        iarchive(World::Get()->component_manager);
    }
    World::RegisterComponent<StaticMeshComponent>(RenderStaticMeshComponent);
    World::RegisterComponent<MaterialComponent>(RenderMaterialComponent);
    World::RegisterComponent<TransformComponent>(RenderTransformComponent);
    World::RegisterComponent<LightComponent>(RenderLightComponent);
    World::RegisterComponent<CameraComponent>(RenderCameraComponent);
    World::RegisterComponent<ControllerComponent>(RenderControllerComponent);
    World::RegisterComponent<SkyBoxComponent>(RenderSkyboxComponent);
    std::cout << testEntityWithTransform.GetComponent<MaterialComponent>()
                     .GetTextures()
                     .at(0)
              << std::endl;

    // while (!vultr->should_close)
    // {
    //     vultr->UpdateGame(lastTime);
    //     glfwSwapBuffers(vultr->window);
    // }
    // vultr->Destroy();
}
