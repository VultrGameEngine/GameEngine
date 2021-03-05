#include <sandbox_test.h>

void SandboxTest::Init()
{
    World::ChangeWorld(World::Init());

    engine->RegisterComponents();

    engine->InitSystems();
    Entity camera = Entity::New();
    CameraComponent camera_component = CameraComponent::Create();
    camera.AddComponent(camera_component);
    camera.AddComponent(ControllerComponent::Create());

    camera.AddComponent(TransformComponent::Create(
        glm::vec3(0, 0, 0), glm::quat(1, 0, 0, 0), glm::vec3(5, 5, 5)));

    Entity light = Entity::New();

    light.AddComponent(LightComponent::Create());

    light.AddComponent(TransformComponent::Create(
        glm::vec3(0, 0, -10), glm::quat(1, 0, 0, 0), glm::vec3(4, 4, 4)));
    light.AddComponent(Vultr::UnlitMaterial::Create());
    light.AddComponent(StaticMeshComponent::Create());
}

void SandboxTest::Update(Vultr::UpdateTick tick)
{
    printf("%f ms\n", tick.m_delta_time);
}

void SandboxTest::Flush()
{
}

Game *init(Vultr::Engine *engine)
{
    return new SandboxTest(engine);
}
void flush(Game *game)
{
    delete game;
}
