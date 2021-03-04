#include <systems/board_system.h>
#include <components/cell_component.h>
#include <chrono>
#include <thread>
#include <math.h>
#include <stdlib.h> /* srand, rand */
#include <time.h>   /* time */
#include <glm/geometric.hpp>

double GetDistance(double fromx, double fromy, double tox, double toy)
{
    return sqrt(pow(fromx - tox, 2) + pow(fromy - toy, 2));
}

glm::vec3 lerp(glm::vec3 x, glm::vec3 y, float t)
{
    if (x == y)
        return y;
    return x * (1.f - t) + y * t;
}

void BoardSystem::OnCreateEntity(Entity entity)
{
    auto &cell_component = entity.GetComponent<CellComponent>();
    BoardSystemProvider &provider = GetProvider();
    provider.m_entities[cell_component.row][cell_component.col][cell_component.zol] =
        entity;
}

void BoardSystem::OnDestroyEntity(Entity entity)
{
    auto &cell_component = entity.GetComponent<CellComponent>();
    BoardSystemProvider &provider = GetProvider();
    provider.m_entities[cell_component.row][cell_component.col][cell_component.zol] =
        Entity(-1);
}

void BoardSystem::Init(int rows, int cols, int zols)
{
    BoardSystemProvider &provider = GetProvider();
    srand(time(NULL));
    glm::vec3 center = glm::vec3(cols / 2 - 0.5, rows / 2 - 0.5, zols / 2 - 0.5);
    glm::vec3 corner = glm::vec3(cols, rows, zols);
    double max_dist = glm::length(corner - center);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            for (int k = 0; k < zols; k++)
            {
                double val = rand() % (int)max_dist;
                double probability =
                    max_dist / (glm::length(glm::vec3(j, i, k) - center) + 0.75);
                if (val < probability)
                {
                    provider.GetCell(i, j, k).GetComponent<CellComponent>().state =
                        true;
                }
            }
        }
    }
    // provider.GetCell(rows / 2, cols / 2).GetComponent<CellComponent>().state =
    // true; provider.GetCell(rows / 2, cols / 2 +
    // 1).GetComponent<CellComponent>().state =
    //     true;
    // provider.GetCell(rows / 2, cols / 2 - 1).GetComponent<CellComponent>().state =
    //     true;
    // provider.GetCell(rows / 2 - 1, cols / 2 + 1)
    //     .GetComponent<CellComponent>()
    //     .state = true;
    // provider.GetCell(rows / 2 - 2, cols / 2).GetComponent<CellComponent>().state =
    //     true;
}

void BoardSystem::StartUpdate()
{
    BoardSystemProvider &provider = GetProvider();
    while (true)
    {
        provider.update = true;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void BoardSystem::RegisterSystem()
{
    Signature signature;
    signature.set(World::GetComponentType<CellComponent>(), true);
    signature.set(World::GetComponentType<TransformComponent>(), true);
    World::RegisterSystem<BoardSystemProvider>(signature);
}

void BoardSystem::Update()
{
    BoardSystemProvider &provider = GetProvider();
    if (!provider.update)
        return;
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            for (int k = 0; k < ZOLS; k++)
            {
                Entity entity = provider.GetCell(i, j, k);
                // If something is horribly wrong
                if (entity.id == -1)
                    continue;

                auto &cell_component = entity.GetComponent<CellComponent>();
                int neighbors = GetNeighborsCount(cell_component);
                if (cell_component.state)
                {
                    if (neighbors < 2)
                    {
                        provider.update_queue.push(
                            QueuedCellUpdate(entity, cell_component.Toggle()));
                    }
                    else if (neighbors == 2 || neighbors == 3)
                    {
                        continue;
                    }
                    else if (neighbors > 3)
                    {
                        provider.update_queue.push(
                            QueuedCellUpdate(entity, cell_component.Toggle()));
                    }
                }
                else
                {
                    if (neighbors == 3)
                    {
                        provider.update_queue.push(
                            QueuedCellUpdate(entity, cell_component.Toggle()));
                    }
                }
            }
        }
    }

    CommitQueue();
    provider.update = false;
}

void BoardSystem::CommitQueue()
{
    BoardSystemProvider &provider = GetProvider();
    while (!provider.update_queue.empty())
    {
        QueuedCellUpdate update = provider.update_queue.front();
        auto &component = update.entity.GetComponent<CellComponent>();
        component = update.component;
        provider.update_queue.pop();
    }
}

void BoardSystem::PrepareBoardForDisplay()
{
    BoardSystemProvider &provider = GetProvider();

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            for (int k = 0; k < ZOLS; k++)
            {
                Entity entity = provider.GetCell(i, j, k);
                // If something is horribly wrong
                if (entity.id == -1)
                    continue;

                auto &cell_component = entity.GetComponent<CellComponent>();
                auto &transform_component =
                    entity.GetComponent<TransformComponent>();
                if (cell_component.state)
                {
                    transform_component.scale =
                        lerp(transform_component.scale, glm::vec3(1, 1, 1), 0.1);
                }
                else
                {
                    transform_component.scale =
                        lerp(transform_component.scale, glm::vec3(0, 0, 0), 0.1);
                }
            }
        }
    }
}

bool BoardSystem::IsAlive(int row, int col, int zol)
{
    BoardSystemProvider &provider = GetProvider();
    Entity entity = provider.GetCell(row, col, zol);
    if (entity.id == -1)
        return false;
    auto &cell_component = entity.GetComponent<CellComponent>();
    return cell_component.state;
}

int BoardSystem::GetNeighborsCount(const CellComponent &component)
{
    BoardSystemProvider &provider = GetProvider();
    int count = 0;
    int zolval[3] = {0, 1, -1};
    for (int i = 0; i < 3; i++)
    {
        if (IsAlive(component.row - 1, component.col, component.zol + zolval[i]))
            count++;
        if (IsAlive(component.row + 1, component.col, component.zol + zolval[i]))
            count++;
        if (IsAlive(component.row, component.col - 1, component.zol + zolval[i]))
            count++;
        if (IsAlive(component.row, component.col + 1, component.zol + zolval[i]))
            count++;
        if (IsAlive(component.row - 1, component.col - 1, component.zol + zolval[i]))
            count++;
        if (IsAlive(component.row - 1, component.col + 1, component.zol + zolval[i]))
            count++;
        if (IsAlive(component.row + 1, component.col - 1, component.zol + zolval[i]))
            count++;
        if (IsAlive(component.row + 1, component.col + 1, component.zol + zolval[i]))
            count++;
    }
    if (IsAlive(component.row, component.col, component.zol + 1))
        count++;
    if (IsAlive(component.row, component.col, component.zol - 1))
        count++;
    return count;
}
