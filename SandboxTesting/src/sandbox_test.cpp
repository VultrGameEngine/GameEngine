#include <sandbox_test.h>
#include <components/test_component.h>
#include <systems/test_system.h>

void SandboxTest::Init()
{
    change_world(new_world());

    engine_register_default_components(engine_global());
    register_component<TestComponent>();

    engine_init_default_systems(engine_global());
    TestSystem::register_system();

    Entity camera = create_entity(get_current_world());
    CameraComponent camera_component = CameraComponent::Create();
    entity_add_component(camera, camera_component);
    entity_add_component(camera, ControllerComponent::Create());

    entity_add_component(camera, TransformComponent::Create(glm::vec3(0, 0, 0), glm::quat(1, 0, 0, 0), glm::vec3(5, 5, 5)));
    entity_add_component(camera, SkyBoxComponent::Create("default", "res/textures/skybox/front.jpg", "res/textures/skybox/back.jpg", "res/textures/skybox/top.jpg", "res/textures/skybox/bottom.jpg",
                                                         "res/textures/skybox/left.jpg", "res/textures/skybox/right.jpg"));
    entity_add_component(camera, Vultr::SkyboxMaterial::Create("default"));

    Entity light = create_entity(get_current_world());

    entity_add_component(light, LightComponent::Create());

    entity_add_component(light, TransformComponent::Create(glm::vec3(0, 0, 30), glm::quat(1, 0, 0, 0), glm::vec3(4, 4, 4)));
    entity_add_component(light, Vultr::UnlitMaterial::Create());

    for (int i = 0; i < 100; i++)
    {
        Entity cube = create_entity(get_current_world());
        entity_add_component(cube, TransformComponent::Create(glm::vec3(0, 0, 2 * i), glm::quat(1, 0, 0, 0), glm::vec3(1, 1, 1)));
        entity_add_component(cube, StaticMeshComponent::Create());
        entity_add_component(cube, Vultr::ForwardMaterial::Create("res/textures/cube/diffuse.png"));
    }
}

void SandboxTest::Update(const Vultr::UpdateTick &tick)
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
