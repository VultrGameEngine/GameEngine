#include <game_of_life.h>
#include <components/cell_component.h>
#include <thread>
#include <systems/board_system.h>
#include <systems/controller_system.h>

void GameOfLife::Init()
{
    World::ChangeWorld(World::Init());

    engine->RegisterComponents();
    World::RegisterComponent<CellComponent>();

    engine->InitSystems();
    BoardSystem::RegisterSystem();
    ControllerSystem::RegisterSystem();
    std::cout << "Test Game initialized... " << std::endl;
    Entity camera = Entity::New();
    CameraComponent camera_component = CameraComponent::Create();
    camera_component.zfar = 400;
    camera.AddComponent(camera_component);
    camera.AddComponent(ControllerComponent::Create());

    camera.AddComponent(TransformComponent::Create(
        glm::vec3(25, 25, 0), glm::quat(1, 0, 0, 0), glm::vec3(5, 5, 5)));

    Entity light = Entity::New();

    light.AddComponent(LightComponent::Create());

    light.AddComponent(TransformComponent::Create(
        glm::vec3(ROWS * 5 / 2, COLS * 5 / 2, ZOLS * -5 / 2), glm::quat(1, 0, 0, 0),
        glm::vec3(4, 4, 4)));
    light.AddComponent(Vultr::UnlitMaterial::Create());

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            for (int k = 0; k < ZOLS; k++)
            {
                Entity cell = Entity::New();
                TransformComponent transform = TransformComponent::Create();
                transform.position = glm::vec3(i * 5, j * 5, k * -5);
                transform.scale = glm::vec3(2, 2, 2);
                cell.AddComponent(transform);
                cell.AddComponent(CellComponent::Create(i, j, k));
                cell.AddComponent(StaticMeshComponent::Create());
                cell.AddComponent(Vultr::ForwardMaterial::Create(
                    "res/textures/cube/cube_texture.jpg"));
            }
        }
    }
    BoardSystem::Init(ROWS, COLS, ZOLS);
    ControllerSystem::Init(glm::vec3(ROWS * 5 / 2, COLS * 5 / 2, ZOLS * -5 / 2),
                           engine->window);
    std::thread update_thread = std::thread(&BoardSystem::StartUpdate);
    update_thread.detach();
}

void GameOfLife::Update(Vultr::UpdateTick tick)
{
    ControllerSystem::Update(tick.m_delta_time);
    BoardSystem::Update();
    BoardSystem::PrepareBoardForDisplay();
    printf("%f ms\n", tick.m_delta_time);
}

void GameOfLife::Flush()
{
}

Game *init(Vultr::Engine *engine)
{
    return new GameOfLife(engine);
}
void flush(Game *game)
{
    delete game;
}
